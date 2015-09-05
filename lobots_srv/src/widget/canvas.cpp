#include "canvas.h"
#include <stdlib.h>
#include "support/splitstr.h"
#include "support/raster.h"
#include "support/easybmp/EasyBMP.h"

TCanvas::TCanvas(): TWindow() {
    Construct();
}

TCanvas::TCanvas( TWidget *Parent ) : TWindow(Parent) {
    Construct();
}

void TCanvas::Construct() {
    draggable = false;
    Width = 0;
    Height = 0;
    canvasData = NULL;
}

const char* TCanvas::GetType() {
    return "canvas";
}

void TCanvas::Resize(int W, int H) {
    TWindow::Resize(W, H);
    if (canvasData != NULL) free(canvasData);
    SetBufferSize(W, H);
    //printf("Resize W %d H %d\n", W, H);
}

void TCanvas::LoadFromXML(XMLElement *elementPointer) {
    TWindow::LoadFromXML(elementPointer);
    
    //image content
    if (elementPointer->FirstChildElement("image") != NULL) {
        //change current pointer
        elementPointer = elementPointer->FirstChildElement("image");
        
        //load from file
        if (elementPointer->FirstChildElement("file") != NULL) {
            const char *fileName = elementPointer->FirstChildElement("file")->GetText();
            LoadImageFromFile(fileName);
        }
        
        //pixels
        elementPointer = elementPointer->FirstChildElement("pixel");
        while (elementPointer != NULL) {
            
            char xstr[30], ystr[30], zstr[30];
            int x, y;
            unsigned char R, G, B, A;
            
            splitstr(elementPointer->GetText(), ',', xstr, ystr);
            x = atoi(xstr);
            splitstr(ystr, ',', xstr, zstr);
            y = atoi(xstr);
            splitstr(zstr, ',', xstr, ystr);
            R = atoi(xstr);
            splitstr(ystr, ',', xstr, zstr);
            G = atoi(xstr);
            splitstr(zstr, ',', xstr, ystr);
            B = atoi(xstr);
            splitstr(ystr, ',', xstr, zstr);
            if (strlen(xstr) > 0) A = atoi(xstr);
            else A = 255;
            
            SetPixel(x, y, R, G, B, A);
            elementPointer = elementPointer->NextSiblingElement("pixel");
        }
        
    }
    
    
}

void TCanvas::Draw(int w, int h) {
    /*
    int x, y;
    unsigned char R, G, B, A;
    for (int i = 0; i < Width*Height; i++) {
        x = i % Width;
        y = i / Width;
        R = canvasData[i].R;
        G = canvasData[i].G;
        B = canvasData[i].B;
        A = canvasData[i].A;
        Raster::SetColor(R, G, B, A);
        Raster::PutPixel(GetScreenX() + x, GetScreenY() + y, w, h);
    }
    */
    
    /*
    int x, y;
    unsigned char R, G, B, A;
    for (int i = 0; i < Width*Height; i++) {
        x = i % Width;
        y = i / Width;
        R = ((unsigned char*) canvasData)[i*4];
        G = ((unsigned char*) canvasData)[i*4+1];
        B = ((unsigned char*) canvasData)[i*4+2];
        A = ((unsigned char*) canvasData)[i*4+3];
        Raster::SetColor(R, G, B, A);
        Raster::PutPixel(GetScreenX() + x, GetScreenY() + y, w, h);
    }*/
    
    Raster::DrawImage(GetScreenX(), GetScreenY(), Width, Height, ((unsigned char*) canvasData), w, h);
}

void TCanvas::SetPixel(int x, int y, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        int canvasDataIndex = y*Width + x;
        canvasData[canvasDataIndex].R = R;
        canvasData[canvasDataIndex].G = G;
        canvasData[canvasDataIndex].B = B;
        canvasData[canvasDataIndex].A = A;
    }
}

bool TCanvas::GetPixel(int x, int y, unsigned char *R, unsigned char *G, unsigned char *B, unsigned char *A) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        int canvasDataIndex = y*Width + x;
        *R = canvasData[canvasDataIndex].R;
        *G = canvasData[canvasDataIndex].G;
        *B = canvasData[canvasDataIndex].B;
        *A = canvasData[canvasDataIndex].A;
        
        return true;
    } else {
        return false;
    }
    
}

void TCanvas::LoadImageFromFile(const char *fileName) {
    BMP *Image = new BMP();
    Image->ReadFromFile(fileName);
    
    int w, h;
    if (Width < Image->TellWidth()) w = Width; else w = Image->TellWidth();
    if (Height < Image->TellHeight()) h = Height; else h = Image->TellHeight();
    
    //printf("LoadImageFromFile W %d H %d\n", w, h);
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            RGBApixel P = Image->GetPixel(x, y);
            SetPixel(x, y, P.Red, P.Green, P.Blue, P.Alpha);
        }
    }    
    
    
    delete(Image);
    
}


void TCanvas::SetBufferSize(int width, int height) {
    canvasData = (struct canvasData*) malloc(sizeof(struct canvasData)*width*height);
    for (int i = 0; i < width*height; i++) {
        canvasData[i].R = 255;
        canvasData[i].G = 255;
        canvasData[i].B = 255;
        canvasData[i].A = 255;
    }
}

