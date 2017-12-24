#include "config.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "opencv2/opencv.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    cv::VideoCapture cap(0); // open the video file for reading
    if(!cap.isOpened())
    {
        return 0;
    }
    
    sf::RenderWindow window(sf::VideoMode(1200, 800), "RenderWindow");
    cv::Mat frameRGB, frameRGBA;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    double zoom = 1.1;
    
    auto size = window.getSize();
    cout << size.x << " " << size.y << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A){
                    zoom = zoom*1.2;
                } else if (event.key.code == sf::Keyboard::B){
                    zoom = zoom*0.8;
                    if(zoom < 1.1){
                        zoom = 1.1;
                    }
                }
            }
            
        }
        
        
        cap >> frameRGB;
        cv::Mat im = frameRGB;
        
        if(frameRGB.empty())
        {
            break;
        }
        cv::Point centre((im.cols-1)/2, (im.rows-1)/2);
        int width = int(im.cols/(2.0*zoom));
        int height = int(im.rows/(2.0*zoom));
        cv::Rect rect(centre.x-width, centre.y-height, width*2, height*2);
        cv::Mat dst = im(rect);
        //
        
        
        cv::Mat res(size.y-5, size.x-5, CV_8UC4, cv::Scalar(0, 0, 1, 255));
        cv::resize(dst, res, res.size(), 0, 0, cv::INTER_LINEAR );
        
        cv::cvtColor(res, frameRGBA, cv::COLOR_BGR2RGBA);
        
        image.create(frameRGBA.cols, frameRGBA.rows, frameRGBA.ptr());
       
        if (!texture.loadFromImage(image))
        {
            break;
        }
        
        sprite.setTexture(texture);
        
        window.draw(sprite);
        window.display();
    }
}
