#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

struct Point{
    float x;
    float y;
};

Point grads[4] = {
    {1,1}, {-1,1} , {-1,-1}, {1,-1}
};

//Point grads[4] = {
//    {1.4,0}, {0,1.4} , {-1.4,0}, {0,-1.4}
//};

float fadeFunc(float x){
    
    return 6*pow(x, 5) - 15*pow(x, 4) + 10*pow(x, 3);
    
}

//float dotProd(float x1, float y1, float x2, float y2){
//    return x1*x2 + y1*y2;
//}

float dotProd(Point p1, Point p2){
    return p1.x * p2.x + p1.y * p2.y;
}


std::vector<std::vector<float>> generatePerlinNoise(int width, int height, int pixelsPerCell) {
    
    //increasing dimensions of vector in to accomodate the the excessive pixels
    if (width % pixelsPerCell != 0) {
        width += pixelsPerCell - (width % pixelsPerCell);
    }
    
    if (height % pixelsPerCell != 0) {
        height += pixelsPerCell - (height % pixelsPerCell);
    }
    std::vector<std::vector<float>> noise(width, std::vector<float>(height, 0.0f));

    /*
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width ; x++) {
            noise[x][y] = 0.5;
        }
    }
    */
    
    float stepPerPixel =  (1 / (float)pixelsPerCell);
    int gridWidth = width / pixelsPerCell;
    int gridHeight = height / pixelsPerCell;
    
    std::vector<std::vector<Point>> gradients;
    
    gradients.resize(gridWidth + 1);
    for (int i = 0; i < gridWidth + 1; i++) {
        gradients[i].resize(gridHeight + 1);
    }
    
    std::srand(static_cast<unsigned int>(std::time(0)));


    for (int x = 0; x < gradients.size(); x++) {
        for (int y = 0; y < gradients[x].size(); y++) {
            int rNum = rand() % 4;
            cout << rNum << endl;
            gradients[x][y] = grads[rNum];
            cout << gradients[x][y].x << " " << gradients[x][y].y << endl;
        }
    }
    
    cout << width << " " << height << endl;
    cout << gridWidth << " " << gridHeight << endl;
    
//    int xOffset = 0, yOffset = 0;
//    int currPixel = 0;
//    while (xOffset != width && yOffset != height) {
//        if (xOffset == width) {
//            xOffset = 0;
//        }
//        
//        int xCell =
//        
//        
//        
//        yOffset += pixelsPerCell;
//        xOffset += pixelsPerCell;
//    }
    
    for (int x = 0; x < noise.size(); x++) {
        for (int y = 0; y < noise[x].size(); y++) {
            
            
            
            //Point currPixel = {(float)(x % pixelsPerCell), (float)(y % pixelsPerCell)};
            float xRel = x % pixelsPerCell;
            float yRel = y % pixelsPerCell;
            
            Point v00 = {xRel*stepPerPixel, yRel*stepPerPixel};
            Point v10 = {-(1-xRel*stepPerPixel), yRel*stepPerPixel};
            Point v01 = {xRel*stepPerPixel, -(1-yRel*stepPerPixel)};
            Point v11 = {-(1-xRel*stepPerPixel), -(1-yRel*stepPerPixel)};
            
            
            
            int currCellX = x/pixelsPerCell;
            int currCellY = y/pixelsPerCell;
            //cout << currCellX << " " << currCellY << endl;
            Point grad00 = gradients[currCellX][currCellY];
            Point grad10 = gradients[currCellX+1][currCellY];
            Point grad01 = gradients[currCellX][currCellY+1];
            Point grad11 = gradients[currCellX+1][currCellY+1];
            
            
            
            float interpolationX0 = dotProd(grad00, v00) + fadeFunc(v00.x) * (dotProd(grad10, v10) - dotProd(grad00, v00));

            float interpolationX1 = dotProd(grad01, v01) + fadeFunc(v00.x) * (dotProd(grad11, v11) - dotProd(grad01, v01));
            //interpolation in Y
            float finalVal = interpolationX0 + fadeFunc(v00.y) * (interpolationX1 - interpolationX0);
            
//            cout << "pixel: " << x << " " << y << endl;
//            cout << "cell: " << currCellX << " " << currCellY << endl;
//            cout << "grad00: " << grad00.x << " " << grad00.y << endl;
//            cout << "grad10: " << grad10.x << " " << grad10.y << endl;
//            cout << "grad01: " << grad01.x << " " << grad01.y << endl;
//            cout << "grad11: " << grad11.x << " " << grad11.y << endl << endl;
            
           // cout << "Point: " << currPixel.x << " " << currPixel.y << endl;
            //cout << "Dot Prod: " << dotProd(grad00, currPixel) << endl;
            
            
            
            
            noise[x][y] = finalVal;
            
            cout << finalVal << endl;
        }
    }
    
    
    
    return noise;
}

int main(int argc, char* argv[]) {
//    int width = 512;  // Width of the window and the noise map
//    int height = 512; // Height of the window and the noise map
    int width = 500;
    int height = 500;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Perlin Noise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Generate Perlin noise values
    std::vector<std::vector<float>> noise = generatePerlinNoise(width, height, 100);

    // Create a texture to render the noise
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Convert Perlin noise values to RGB pixels
    std::vector<Uint32> pixels(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = (noise[x][y] + 1)/2;
            Uint8 color = static_cast<Uint8>(value * 255);
            pixels[y * width + x] = (255 << 24) | (color << 16) | (color << 8) | color; // ARGB format
        }
    }

    // Update texture with pixel data
    SDL_UpdateTexture(texture, nullptr, pixels.data(), width * sizeof(Uint32));

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Render the texture
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        // Present the rendered image
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

