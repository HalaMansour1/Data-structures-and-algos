


#include "Image.h"
#include "cs225/PNG.h"
#include <cmath>
#include "cs225/HSLAPixel.h"



//Image::Image(unsigned int width, unsigned int height){



void Image::darken(){
    int x = width();
    int y = height();
    for( int i=0; i<x; i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).l >= 0.1){
            getPixel(i,j).l = getPixel(i,j).l - 0.1 ;
          }else{
            getPixel(i,j).l = 0;
          }
        
        }
    }

}	
void Image::darken(double amount){
    int x = width();
    int y = height(); 
for( int i=0;i<x;i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).l >= amount){
            getPixel(i,j).l = (getPixel(i,j).l) - amount ;
          }else{
            getPixel(i,j).l = 0;
          }
        
        }
    }
}
void Image::desaturate(){
     int x = width();
    int y = height();
for(int i=0;i<x;i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).s >= 0.1){
            getPixel(i,j).s = getPixel(i,j).s - 0.1 ;
          }else{
            getPixel(i,j).s = 0;
          }
        
        }
    }
}
void Image::desaturate(double amount){
     int x = width();
    int y = height();
for( int i=0;i<x;i++){
        for(int j=0;j<y;j++){
          if (getPixel(i,j).s >= amount){
            getPixel(i,j).s = getPixel(i,j).s - amount ;
          }else{
            getPixel(i,j).s = 0;
          }
        
        }
    }
}
void Image::grayscale(){
     int x = width();
    int y = height();
for( int i=0;i<x;i++){
        for( int j=0;j<y;j++){
               getPixel(i,j).s = 0; 
        }
}
}
void Image::illinify(){
     int x = width();
    int y = height();
    const int o_hue = 11;
    const int b_hue = 216; 
    for( int i=0;i<x;i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).h < ((o_hue+b_hue)/2)){
            getPixel(i,j).h = o_hue ;
          }else{
            getPixel(i,j).h = b_hue;
          }
        
        }
    }

}
void Image::lighten	(){
     int x = width();
    int y = height();
for( int i=0;i<x;i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).l <= 0.9){
            getPixel(i,j).l = getPixel(i,j).l + 0.1 ;
          }else{
            getPixel(i,j).l = 1;
          }
        
        }
    }
}
void Image::lighten	(double amount){
     int x = width();
    int y = height();
    for( int i=0;i<x;i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).l <= 1- amount){
            getPixel(i,j).l = getPixel(i,j).l + amount ;
          }else{
            getPixel(i,j).l = 1;
          }
        
        }
    }

}
void Image::rotateColor(double degrees){
     int x = width();
    int y = height();
for(int i=0;i<x;i++){
        for( int j=0;j<y;j++){
            if((getPixel(i,j).h + degrees) < 0){
              getPixel(i,j).h =  360 - (std::fmod(std::abs(getPixel(i, j).h + degrees), 360)); 
            }else{
                getPixel(i,j).h = std::fmod((getPixel(i, j).h + degrees), 360); 
            }
        }
    }
}
void Image::saturate(){
     int x = width();
    int y = height();
for( int i=0; i<x; i++){
        for( int j=0;j<y;j++){
          if (getPixel(i,j).s <= 0.9){
            getPixel(i,j).s = getPixel(i,j).l + 0.1 ;
          }else{
            getPixel(i,j).s = 1;
          }
        
        }
    }
}
void Image::saturate(double amount)	{
for(unsigned int i=0; i<width(); i++){
        for(unsigned int j=0;j<height();j++){
          if (getPixel(i,j).s <= 1 - amount){
            getPixel(i,j).s = getPixel(i,j).l + amount ;
          }else{
            getPixel(i,j).s = 1;
          }
        
        }
    }
}
void Image::scale(double factor) {
    
    Image imageCopy(*this);
    
    unsigned int w = width() * factor;   
    unsigned int h = height() * factor;

    resize(w,h); 

    unsigned int w_, h_;
    for(unsigned int i=0; i<w; i++) {
        for (unsigned int j=0; j<h; j++) {
            // fill pixels from old image into new image
            w_ = (unsigned int) i/factor;
            h_ = (unsigned int) j/factor;
            this->getPixel(i,j) = imageCopy.getPixel(w_,h_);
        }
    }
   
}
void Image::scale(unsigned w, unsigned h) {
    
    
    double wfactor = (double) w/ width();
    double hfactor = (double) h/ height();
    double factor = std::min(wfactor, hfactor);

    scale(factor);
}




