//
// Created by sarkar on 20.04.15.
//

#ifndef OBJECT_DETECTION_FEATUREDETECTOR_H
#define OBJECT_DETECTION_FEATUREDETECTOR_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "SiftGPU.h"

#include <opencv2/cudafeatures2d.hpp>

using namespace std;

class KFeatureDetector {

public:
  KFeatureDetector(string feature_type, bool use_gpu);

  enum FeatureType {
    SIFT,
    SURF,
    ORB,
    SIFT_GPU,
    ORB_GPU,
  };

  int mode_;

  KFeatureDetector(FeatureType type)
  {

    mode_ = type;

    if (type == SIFT) {
      feature_detector_ = cv::xfeatures2d::SIFT::create();
    }
    else if (type == ORB) {
      feature_detector_ = cv::ORB::create();
    }
    else if (type == SURF) {
      feature_detector_ = cv::xfeatures2d::SURF::create();
    }
    else if (type == ORB_GPU) {
      feature_detector_ = cv::cuda::ORB::create();
    }
    else if (type == SIFT_GPU) {
      sift_gpu_ = new SiftGPU;
      //char * argv[] = {(char *)"-fo", (char *)"-1",  (char *)"-v", (char *)"1"};
      char * argv[] = {(char *)"-fo", (char *)"-1",  (char *)"-v", (char *)"3", (char *)"-cuda"};
      int argc = sizeof(argv)/sizeof(char*);
      sift_gpu_->ParseParam(argc, argv);
      if(sift_gpu_->CreateContextGL() != SiftGPU::SIFTGPU_FULL_SUPPORTED)
        cout << "FATAL ERROR cannot create SIFTGPU context";
    }
  }

  //always return Opencv type keypoints
  void computeKeypointsAndDescriptors(cv::Mat const &image, cv::Mat &descriptors, vector<cv::KeyPoint> &keypoints);

  void findSiftGPUDescriptors(char const *image_name, cv::Mat &descriptors, vector<cv::KeyPoint> &keypoints);

  void findSiftGPUDescriptors(cv::Mat const &image, cv::Mat &descriptors, vector<cv::KeyPoint> &keypoints);

private:
  //cv::Ptr<cv::FeatureDetector> detector_;
  // pointer to the feature descriptor extractor object
  //cv::Ptr<cv::DescriptorExtractor> extractor_;

  cv::Ptr<cv::FeatureDetector> feature_detector_;
  cv::Ptr<SiftGPU> sift_gpu_;


  void findSiftGPUDescriptors1(cv::Mat const &image, cv::Mat &descriptors, vector<cv::KeyPoint> &keypoints);
};

#endif //OBJECT_DETECTION_FEATUREDETECTOR_H
