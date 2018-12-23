include $(project_root)/etc/makefiles/deep_leaning_defines.mk

ifeq ($(OPENCV_VERSION),'3.2.0',) 
OPENCV_LIBS_LIST := opencv_shape opencv_stitching \
    opencv_objdetect opencv_superres \
    opencv_videostab opencv_calib3d \
    opencv_features2d opencv_highgui \
    opencv_videoio opencv_imgcodecs \
    opencv_video opencv_photo opencv_ml \
    opencv_imgproc opencv_flann opencv_core
else
    ifeq ($(OPENCV_VERSION),'3.3.0')
      OPENCV_LIBS_LIST := opencv_dnn opencv_ml opencv_objdetect \
        opencv_shape opencv_stitching opencv_superres \
        opencv_videostab opencv_calib3d opencv_features2d \
        opencv_highgui opencv_videoio opencv_imgcodecs \
        opencv_video opencv_photo opencv_imgproc \
        opencv_flann opencv_core \
        ittnotify libprotobuf libjpeg libwebp \
        libpng libtiff libjasper IlmImf \
        gomp dl z m pthread rt
     else
       OPENCV_LIBS_LIST := opencv_dnn opencv_ml opencv_objdetect \
	opencv_shape opencv_stitching opencv_superres \
	opencv_videostab opencv_calib3d opencv_features2d \
	opencv_highgui opencv_videoio opencv_imgcodecs \
	opencv_video opencv_photo opencv_imgproc \
	opencv_flann opencv_core \
	ittnotify libprotobuf \
	libjpeg libwebp libpng \
	libtiff libjasper IlmImf \
	dl z m pthread rt
     endif
endif
