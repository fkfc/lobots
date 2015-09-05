#ifndef canvas_H
#define canvas_H

#include "window.h"
#include "widget.h"

class TCanvas : public TWindow {
    public:
        TCanvas();
        TCanvas( TWidget *Parent );
        virtual const char* GetType();
        virtual void Resize(int W, int H);
        void LoadFromXML(XMLElement *elementPointer);
        void Draw(int w, int h);
        void SetPixel(int x, int y, unsigned char R, unsigned char G, unsigned char B, unsigned char A);
        bool GetPixel(int x, int y, unsigned char *R, unsigned char *G, unsigned char *B, unsigned char *A);
        void LoadImageFromFile(const char *fileName);

    private:
        void Construct();
        void SetBufferSize(int width, int height);
        
        struct canvasData {
            unsigned char R, G, B, A;
        } *canvasData;
        unsigned int glTextureID;
        
        
        
};


#endif
