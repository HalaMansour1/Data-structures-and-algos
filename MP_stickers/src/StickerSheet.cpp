
#include "StickerSheet.h"
#include "cs225/HSLAPixel.h"
#include <iostream>
#include <cmath>

using namespace cs225;

// initialize stickersheet with base picture 
StickerSheet::StickerSheet(const Image &picture) {
    // set base image
    baseImage = picture;
    //stickersheet.push_back(new Image(baseImage));
   
}

// StickerSheet::~StickerSheet() {
//     // no dynamically allocated files
//     for (unsigned int i=0; i<stickersheet.size(); i++) {
//         delete stickersheet.at(i);
//     }
// }


int StickerSheet::addSticker(Image &sticker, int x, int y) {
    // if the sticker array is at max capacity then create new layer above all existing 
   unsigned long max = stickersheet.max_size();
   Sticker tempSticker(sticker, x, y);
    // if (stickersheet.size() != max) {
    //     stickersheet.push_back(tempSticker);
    // }else{
    //     stickersheet.insert(stickersheet.begin(), tempSticker);
    // }
    stickersheet.push_back(tempSticker);
    return stickersheet.size() -1  ; 
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index>=stickersheet.size()) return NULL;
    return (stickersheet[index].stickerImage_);
}

int StickerSheet::layers()const{
return stickersheet.size();
}

void StickerSheet::removeSticker(unsigned index) {
    // delete stickersheet.at(index);
    stickersheet.erase(stickersheet.begin()+index);

}



int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
    if( layer < stickersheet.size()){
        stickersheet[layer] = Sticker(sticker, x, y);
        return layer;
    } 
    stickersheet.push_back(Sticker(sticker, x, y));
    return stickersheet.size()-1;
    // if (layer >= stickersheet.size()) {
    //             stickersheet.insert(stickersheet.end(),new Sticker(sticker, x, y));  
    // }else if( layer < stickersheet.size()){
    //             stickersheet[layer] = Sticker(sticker, x, y);
    // }else{
    //          delete stickersheet[layer];  
    //          stickersheet[layer] = new Sticker(sticker, x, y); 
    //     }
   
    // return (stickersheet.size()) ;  // Return the zero-based layer index
}

bool StickerSheet::translate(unsigned index, unsigned x,  unsigned y) {
    // if index is greater than num of stickers then return false
    if (stickersheet.size() < index) return false;
    // modify x and y values at index
    stickersheet.at(index).x_ = x;
    stickersheet.at(index).y_ = y;
    return true;

}





 Image StickerSheet::render() const {
//         //Image imageCopy(baseImage);
     Image newImage = baseImage;
    int max_x = baseImage.width();
     int max_y = baseImage.height();
     int min_x = 0 ; 
     int min_y = 0 ; 

     for (unsigned int i=0; i<stickersheet.size(); i++) {
//       //  Image *baseImage = imageCopy;
        
          int xcoor= stickersheet.at(i).x_;
          int ycoor= stickersheet.at(i).y_;
          int startWidth, endWidth, startHeight, endHeight, widthCounter, heightCounter;
          startWidth = xcoor;
          endWidth = startWidth+stickersheet.at(i).stickerImage_->width();
          if (endWidth>max_x) {
              max_x = endWidth ;
          } 
          startHeight = ycoor;
          endHeight = startHeight+stickersheet.at(i).stickerImage_->height();
          if (endHeight>max_y) {
               max_y = endHeight ;
          } 
          min_x = std::min(min_x, stickersheet.at(i).x_) ; 
          min_y = std::min(min_y, stickersheet.at(i).y_);

      }
     
      newImage.resize(max_x - min_x, max_y - min_y);
       //make new 
    //Image name(maxxx - minx, maxy- miny);
      // draaw baaseimage onn new
      //for loop base image 
      for (unsigned int i=0; i<stickersheet.size(); i++) {
      int widthCounter= 0 ;
      int heightCounter= 0 ;
      int startWidth = stickersheet.at(i).x_ - min_x ;
      int startHeight = stickersheet.at(i).y_ - min_y ;
      unsigned int  endWidth, endHeight;
      endWidth = startWidth+stickersheet.at(i).stickerImage_->width()   ;
      endHeight = startHeight+stickersheet.at(i).stickerImage_->height()  ;
      for (unsigned int w=startWidth; w<(endWidth); w++) {
          for ( unsigned int h=startHeight; h<(endHeight); h++) {
              if (stickersheet.at(i).stickerImage_->getPixel(widthCounter,heightCounter).a != 0) {
                  newImage.getPixel(w,h) = stickersheet.at(i).stickerImage_->getPixel(widthCounter,heightCounter);
                  heightCounter++;
              } else {
                  heightCounter++;
              }
          }
      heightCounter=0;
      widthCounter++;
      }

   
  } 
   return newImage;
  }

