#include<vector>
#include <iostream>
#include<SDL.h>
#include<SDL_image.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

SDL_Window* window = SDL_CreateWindow("Dijkstra",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
bool quit = false;
bool play_simulation = false;
SDL_Event event;

SDL_Rect rect1 = {25,25,50,50};
SDL_Rect rect2 = {400,45,50,50};
SDL_Rect rect3 = {300,300,50,50};
SDL_Rect rect4 = {560,400,50,50};
SDL_Rect rect5 = {650,350,50,50};


SDL_Rect* selectedRect;//for the drag and drop
SDL_Point position;//position of mouse
SDL_Point clickOffset;//to avoid rectangle jumping to mouse
bool mouseLeftClicked;//to know if we start dragging


// Careful about addresses vectors
// returning values instead of addresses avoid memory leak, but here lets use delete
SDL_Point* getRectCenter(const SDL_Rect& rect){
    auto* point =  new SDL_Point;//Allocated memory is leaked when called from function 'updateCenters'
    point->x = rect.x+rect.w/2;
    point->y = rect.y+rect.h/2;
    return point;
}

void updateCenters(const std::vector<SDL_Rect>& rects,std::vector<SDL_Point>& centers){
        centers.clear();
        for(auto& rect:rects){
//            centers.push_back(*getRectCenter(rect));//Allocated memory is leaked
            SDL_Point* p = getRectCenter(rect);
            centers.push_back(*p);
            delete p;//no more memory leak
        }
}

void linkTwoNodes(SDL_Renderer* p_renderer, const SDL_Point& p1, const SDL_Point& p2){
    SDL_RenderDrawLine(p_renderer,p1.x,p1.y,p2.x,p2.y);
}

int main(int argc,char** argv) {
    // init procedure
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)>0)
    {
        std::cout << "INIT ERROR";
        return 1;
    }
    if(!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "INIT IMG ERROR";
        return 2;
    }


    // Entities Creation
    // this particular piece of code only works in main , why ?
    std::vector<SDL_Rect> my_rects {};
    my_rects.push_back(rect1);
    my_rects.push_back(rect2);
    my_rects.push_back(rect3);
    my_rects.push_back(rect4);
    my_rects.push_back(rect5);

    std::vector<SDL_Point> rects_centers {};
    // loop function
    while(!quit)
    {
        SDL_PollEvent((&event));
        if(!play_simulation){
            switch(event.type) {

            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                position = {event.motion.x, event.motion.y};
                if (selectedRect != nullptr) {
                    selectedRect->x = event.motion.x - clickOffset.x;
                    selectedRect->y = event.motion.y - clickOffset.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (mouseLeftClicked) {
                    mouseLeftClicked = false;
                    selectedRect = nullptr;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseLeftClicked = true;
                for (auto &rect: my_rects) {
                    if (SDL_PointInRect(&position, &rect)) {
                        selectedRect = &rect;//without & (reference) selectedRect = rect The address of the local variable 'rect' may escape the function, for we create a copy
                        //of every rect in this function
                        clickOffset.x = position.x - rect.x;
                        clickOffset.y = position.y - rect.y;
                        break;
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_F) {
                    play_simulation = true;
                    SDL_RenderClear(renderer);
                }
        }
        }//endif play_simulation
        switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_G) {
                        play_simulation = false;
                    }
                    break;
        }

        SDL_SetRenderDrawColor(renderer,125,148,94,0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,255,255,255,0);

        for(const SDL_Rect& rect: my_rects){
            if(&rect == selectedRect){
                SDL_SetRenderDrawColor(renderer,0,0,255,0);
                SDL_RenderFillRect(renderer,&rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer,255,255,255,0);
                SDL_RenderFillRect(renderer,&rect);
            }
//            SDL_RenderDrawLine(renderer,rect1.x+rect1.w/2,rect1.y+rect1.h/2,rect2.x+rect2.w/2,rect2.y+rect2.h/2);//are constants
            SDL_SetRenderDrawColor(renderer,255,255,255,0);
            //SDL_RenderDrawLine(renderer,my_rects[0].x+my_rects[0].w/2,my_rects[0].y+my_rects[0].h/2,my_rects[1].x+my_rects[1].w/2,my_rects[1].y+my_rects[1].h/2);
            updateCenters(my_rects,rects_centers);
            /*
            SDL_RenderDrawLine(renderer,rects_centers[0].x,rects_centers[0].y,rects_centers[1].x,rects_centers[1].y);
            SDL_RenderDrawLine(renderer,rects_centers[0].x,rects_centers[0].y,rects_centers[2].x,rects_centers[2].y);
            SDL_RenderDrawLine(renderer,rects_centers[1].x,rects_centers[1].y,rects_centers[2].x,rects_centers[2].y);
             */

            linkTwoNodes(renderer,rects_centers[0],rects_centers[1]);
            linkTwoNodes(renderer,rects_centers[4],rects_centers[2]);
            linkTwoNodes(renderer,rects_centers[1],rects_centers[2]);
            linkTwoNodes(renderer,rects_centers[3],rects_centers[1]);
            linkTwoNodes(renderer,rects_centers[0],rects_centers[3]);
            linkTwoNodes(renderer,rects_centers[2],rects_centers[3]);

            /*links all nodes
             * for(int i=0;i<my_rects.size()-1;i++){
                for(int j=1;j<my_rects.size();j++){
                    linkTwoNodes(renderer,rects_centers[i],rects_centers[j]);
                }
            }
             */


        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);//not very game friendly, time stepping is the option in case of game
    }

    //Cleanup methods
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
