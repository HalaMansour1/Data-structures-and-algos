/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{

    if(theTiles.empty()){ return NULL; }

    vector<Point<3>> points;
    for (size_t i = 0; i < theTiles.size(); ++i) {
        LUVAPixel luvaColor = theTiles[i].getAverageColor();
        points.push_back(convertToXYZ(luvaColor));
    }
    //cout<<points<<endl;
    

    KDTree<3> tree(points); // making tree and final mosaic 
    //cout<< tree<endl;
    
    MosaicCanvas* final = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    for (int row = 0; row < theSource.getRows(); ++row) {
        for (int col = 0; col < theSource.getColumns(); ++col) { // loop thru each r and c 
         LUVAPixel sourceColor = theSource.getRegionColor(row, col);
        Point<3> search = convertToXYZ(sourceColor);
        Point<3> nearestN = tree.findNearestNeighbor(search);

            vector<Point<3>>::iterator it = std::find(points.begin(), points.end(), nearestN);
            int tile = std::distance(points.begin(), it);
            //cout<< tile<endl;
            TileImage* closesttile = &theTiles[tile];

            //  uses find nearest neighbour
        final->setTile(row, col, closesttile);
        }
    }
    //cout<< tree<endl;
    return final;

}

