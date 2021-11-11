#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <opencv2/imgproc.hpp>

using namespace dlib;
using namespace std;


int main()
{
    try
    {
        cv::VideoCapture cap(1);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        image_window win;

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

        // Grab and process frames until the main window is closed by the user.
        while (!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            if (!cap.read(temp))
            {
                break;
            }

            //DOWNSIZE IMAGE AND TURN INTO GRAYSCALE FOR FASTER PROCESSING
            int scale_percent = 40;
            int width = int(temp.cols * scale_percent / 100);
            int height = int(temp.rows * scale_percent / 100);

            cv::Mat temp_downSize;

            cv::resize(temp, temp_downSize, cv::Size(width, height), cv::INTER_LINEAR);

            //cv::Mat grayscale_temp;
            //cv::cvtColor(temp_downSize, grayscale_temp, cv::COLOR_BGR2GRAY);

            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.

            cv_image<bgr_pixel> cimg(temp);

            dlib::array2d<unsigned char> img_gray;
            dlib::assign_image(img_gray, cimg);


            // Detect faces 
            std::vector<rectangle> faces = detector(img_gray);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i)
                shapes.push_back(pose_model(img_gray, faces[i]));

            // Display it all on the screen
            win.clear_overlay();
            win.set_image(cimg);
            win.add_overlay(render_face_detections(shapes));
        }
    }
    catch (serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
}