
#pragma once
#include "Image.h"
#include <vector>

using namespace cs225;
class StickerSheet {
    public: 
        StickerSheet(const Image &picture);
       
        int addSticker(Image &sticker, int x, int y);
        Image *getSticker(unsigned index);
        int layers()const;
        void removeSticker(unsigned index);
        Image render() const;
        int setStickerAtLayer(Image & 	sticker,unsigned 	layer,int 	x,int 	y )	;
        bool translate(unsigned index, unsigned x,  unsigned y);
        
        
    private:
        struct Sticker {
            Image *stickerImage_;
            int x_;
            int y_;
            Sticker(Image &stickerImage, int x, int y) {
                stickerImage_ = &stickerImage;
                x_ = x;
                y_ = y;
            }
        };
        std::vector<Sticker> stickersheet;
        Image baseImage;
};  
