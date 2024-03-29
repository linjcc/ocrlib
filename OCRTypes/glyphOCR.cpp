﻿#include "config.h"

#include "OCRTypes.h"
#include "glyphOCR.h"
#include <string.h>
#include <fstream>
#include "OCRString/php2stl.h"

// ----- constructor and destructor
//________glyphOCR_________________
glyphOCR::~glyphOCR(){}

void glyphOCR::destroy(void){

}

glyphOCR::glyphOCR(){
		mask32Count=0;
		mask128Count=0;
		vectorCount=0;
	    y0=0;
	    y1=0;
		dX=0;
	    dY=0;
	    OCRIndex=0;
	    recordFlag=1;
	    newLetterInBook=1;
		cn=0;
		cnRes=0;
		crSum=0;
		crRes=0;
	    vectorCount=0;
	    selfCorrelation=0;
	    codeSpace=0;
}

int operator==(const glyphOCR &left, const glyphOCR &right){
	return  left.correlation==right.correlation;
}

int operator<(const glyphOCR &left, const glyphOCR &right){
	return  left.correlation>right.correlation;
}

void glyphOCR::eraseMask(int id){
//cout<<"need erase id="<<id<<" mask32Count="<<mask32Count<<endl;
if(id>=mask32Count)return;  
if(id<0)return;             

mask32[id]=mask32[mask32Count-1];
mask32Count--;

}//_____________________________________________________________________________


void glyphOCR::copy(glyphOCR* letter){
letterIndex=letter->letterIndex;
stringNum=letter->stringNum;
idNumber=letter->idNumber;	
recordFlag=letter->recordFlag;
recordStatus=letter->recordStatus;
lengthSum=letter->lengthSum;
angLength=letter->angLength;

for(int i=0;i<letter->mask32Count;i++){mask32[i]=letter->mask32[i];}
for(int i=0;i<letter->mask128Count;i++){mask128[i]=letter->mask128[i];}

mask32Count=letter->mask32Count;
mask128Count=letter->mask128Count;
vectorCount=letter->vectorCount;
//center=letter->center;
name=letter-> name;
Wylie=letter-> Wylie;
OCRKey=letter-> OCRKey;
OCRIndex=letter->OCRIndex;
correlation =letter->correlation;
Character=letter->Character;
vectorLimit=letter->vectorLimit;
stackFlag=letter->stackFlag;
OCRStatus=letter->OCRStatus;
letterW=letter->letterW;
letterH=letter->letterH;
baseLetter=letter->baseLetter;
xMin=letter->xMin;
xMax=letter->xMax;
yMax=letter->yMax;
yMin=letter->yMin;
xSum=letter->xSum;
ySum=letter->ySum;
scale=letter->scale;
y0=letter->y0;
y1=letter->y1;
newLetter=letter->newLetter;
newLetterInBook=letter->newLetterInBook;
allLetterCount=letter->allLetterCount;
cn=letter->cn;
cnRes=letter->cnRes;
crSum=letter->crSum;
crRes=letter->crRes;		
	
}//_____________________________________________________________________________


void glyphOCR::reloadMask(){
//cout<<"reload mask scale="<<scale<<endl;
	int border=64;
	GBitmap *img=mask128[0].unPackMask(); 
	GBitmap *destImg=GBitmap::create(mask128[0].mW+2*border,mask128[0].mH+2*border);
	destImg->drawImg(img, border, border);
	//img->printToScreen();
	int print=0;
	DT("mask32Count="<<mask32Count<<endl);
	//destImg->printToScreen();
	int x0,y0,w,h;
	//cout<<" done"<<endl;
	//int maskXMin=512,maskYMin=512;

	for(int m=0; m<mask32Count;m++){	
		x0=mask32[m].xMask; 
		y0=mask32[m].yMask;  
		w=mask32[m].imgW;    
		h=mask32[m].mH;
		DT("m="<<m<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl);
		if(destImg->columns()/2+x0<0||
		   destImg->columns()/2+x0+w>destImg->columns()||
		   w<0||
		   destImg->rows()/2+y0<0||
		   destImg->rows()/2+y0+h>destImg->rows()||
		   h<0){cout<<"@@@No VALID MASK "<<m<<"in glyph"<<name<<endl;mask32[m].status=0; continue;}
		//GBitmap *imgMask=GBitmap::create(destImg,destImg->columns()/2+x0+2,destImg->rows()/2+y0+1,w,h);
		GBitmap *imgMask=GBitmap::createRegion(destImg,destImg->columns()/2+x0,destImg->rows()/2+y0,w,h);
		DT("m="<<m<<endl);
		//imgMask->printToScreen();
		DT("imgMask->columns()="<<imgMask->columns()<<" imgMask->rows()="<<imgMask->rows());
	 	mask32[m].packBitMask(imgMask);
		imgMask->destroy();
	}
	
	//Erase empty mask;
	
	int index=0;
	for(int i=0;i<mask32Count;i++){
		if(mask32[i].status){
			mask32[index]=mask32[i];
			mask32[index].status=1;
		    index++;
		}
		
	}	
	mask32Count=index;
	
	DT("done");
};


void glyphOCR::calibrateMask(GBitmap *img){
//http://localhost/cgi/YagpoOCR.cgi
//http://localhost/1.bmp.html
//http://localhost/OCRBase.html

};


void glyphOCR::maskFromVector(){

	int index;
	int maxL;
		
	//cout<<"@xMin="<<xMin<<" yMin="<<yMin<<" xMax="<<xMax<<" yMax="<<yMax<<" textLineSize="<<textLineSize<<endl;
	int sizeDL=textLineSize/15; ///sizeDL - we spread Mask box around vector	

	for(int i=0;i<vectorCount;i++){
		maxL=0; index=-1;
		for(int j=0;j<vectorCount;j++){
			if(v[j].selectFlag)continue;
			if(v[j].length>maxL){maxL=v[j].length;index=j;}
		}
		//cout<<"index="<<index<<endl;
		if(index==-1)break;
        v[index].selectFlag=1;
		
		int dL=v[index].length/10; 
		if(dL>textLineSize/2)dL=textLineSize/2;
		//if(dL<textLineSize/5)dL=textLineSize/3;
		mask32[mask32Count].xMask=v[index].vStart.x-dL;
		if(v[index].vStart.x<v[index].vEnd.x){
			mask32[mask32Count].xMask=v[index].vStart.x-dL;
		}else{
			mask32[mask32Count].xMask=v[index].vEnd.x-dL;		
		}
		

		if(mask32[mask32Count].xMask<xMin-sizeDL)mask32[mask32Count].xMask=xMin-sizeDL;		
		
		if(v[index].vStart.y<v[index].vEnd.y){
		    mask32[mask32Count].yMask=v[index].vStart.y-dL;
		}else{
		    mask32[mask32Count].yMask=v[index].vEnd.y-dL;		
		}	
		
		if(mask32[mask32Count].yMask<yMin)mask32[mask32Count].yMask=yMin;  ///cat Mask box in letterH
		
		mask32[mask32Count].imgW=abs(v[index].vStart.x-v[index].vEnd.x)+dL*2;
		if(mask32[mask32Count].imgW>32){
			mask32[mask32Count].imgW=32;
			mask32[mask32Count].xMask+=(mask32[mask32Count].imgW-32)/2;	
			        ///cat Mask box in 32 (size of ASM slice)
		}
		
		if(mask32[mask32Count].imgW>textLineSize*0.9){
			mask32[mask32Count].imgW=textLineSize*0.9;  
			mask32[mask32Count].xMask+=(mask32[mask32Count].imgW-textLineSize*0.9)/2;	
			      ///cat Mask box in textLineSize*0.7 (not too big in width	)
		}
		
		if(mask32[mask32Count].xMask+mask32[mask32Count].imgW>xMax+sizeDL)mask32[mask32Count].imgW=xMax-mask32[mask32Count].xMask+sizeDL; ///cat Mask box in letterW
		
		mask32[mask32Count].mH=abs(v[index].vStart.y-v[index].vEnd.y)+dL*2;

		if(mask32[mask32Count].mH>textLineSize){
			mask32[mask32Count].yMask+=(mask32[mask32Count].mH-textLineSize)/2;	
			mask32[mask32Count].mH=textLineSize;        ///cat Mask box in 0.7 text line size (not too long mask)
		}
		
		if(mask32[mask32Count].mH>textLineSize*0.7&&mask32[mask32Count].imgW>textLineSize*0.7){  //too big mask
			mask32[mask32Count].mH*=0.75; 
			mask32[mask32Count].imgW*=0.75;   
		}
		
		
		
		if(mask32[mask32Count].yMask+mask32[mask32Count].mH>yMax+sizeDL*2)mask32[mask32Count].mH=yMax-mask32[mask32Count].yMask+sizeDL*2;   //cut mask bottom
				
		mask32Count++; 
	}

   //КОМПАКТИЗАЦИЯ ПРИЗНАКОВ  все признаки находящиеся внутри большого признака им поглощаются
  for(int i=0;i<mask32Count;i++)mask32[i].status=1;
	
  for(int i=0;i<mask32Count;i++){
	  
	  if(!mask32[i].status)continue;
  	  
	  for(int j=0;j<mask32Count;j++){
          
		  if(i==j||!mask32[j].status)continue;
		  if(mask32[j].xMask+textLineSize/6>=mask32[i].xMask&&
			 mask32[j].xMask+mask32[j].imgW<mask32[i].xMask+mask32[i].imgW+textLineSize/6&&

			 mask32[j].yMask+textLineSize/6>=mask32[i].yMask&&
			 mask32[j].yMask+mask32[j].mH<mask32[i].yMask+mask32[i].mH+textLineSize/6
		 
			)  mask32[j].status=0;
	  }	  
  }	


  //Grow small mask
	for(int j=0;j<mask32Count;j++){
		
		if(!mask32[j].status)continue;
		if(mask32[j].imgW<textLineSize/4){
			mask32[j].imgW=textLineSize/4;
			if(mask32[j].xMask+mask32[j].imgW>xMax+sizeDL){
				mask32[j].xMask=xMax-mask32[j].xMask+sizeDL-mask32[j].imgW; ///cat Mask box in letterW
			}	
		}
		if(mask32[j].mH<textLineSize/4){
			mask32[j].mH=textLineSize/4;
			if(mask32[j].yMask+mask32[j].mH>yMax+sizeDL){
				mask32[j].yMask=yMax-mask32[j].yMask+sizeDL-mask32[j].mH; ///cat Mask box in letterW
			}	
		}
		
				
	}	
	   //КОМПАКТИЗАЦИЯ ПРИЗНАКОВ  все признаки находящиеся внутри большого признака им поглощаются
	for(int i=0;i<mask32Count;i++){
		
		if(!mask32[i].status)continue;
		
		for(int j=0;j<mask32Count;j++){
			
			if(i==j||!mask32[j].status)continue;
			if(mask32[j].xMask+textLineSize/6>=mask32[i].xMask&&
			   mask32[j].xMask+mask32[j].imgW<mask32[i].xMask+mask32[i].imgW+textLineSize/6&&
			   
			   mask32[j].yMask+textLineSize/6>=mask32[i].yMask&&
			   mask32[j].yMask+mask32[j].mH<mask32[i].yMask+mask32[i].mH+textLineSize/6
			   
			   )  mask32[j].status=0;
		}	  
	}	
	
	
	
  //Erase empty mask;
	
	index=0;
	for(int i=0;i<mask32Count;i++){
		if(mask32[i].status){
			mask32[index]=mask32[i];
			mask32[index].maskType=1;
			mask32[index].status=1;
			mask32[index].dlt0=8;
			mask32[index].dlt1=1;
			mask32[index].dlt2=0;
			
		    index++;
		}
		
	}	
	mask32Count=index;
	
	
	

};


string glyphOCR::drawToSVG(){
	
	int n=300/textLineSize;
	ostringstream out;
	int d=textLineSize;
	int color;
	out<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"<<endl;
	out<<"<svg version = \"1.1\""<<endl;
    out<<"baseProfile=\"full\""<<endl;
	out<<"xmlns = \"http://www.w3.org/2000/svg\""<<endl;
	out<<"xmlns:xlink = \"http://www.w3.org/1999/xlink\""<<endl;
	out<<"xmlns:ev = \"http://www.w3.org/2001/xml-events\""<<endl;
	out<<"height = \"800px\"  width = \"800px\">"<<endl;
	out<<"<rect x=\"0\" y=\"0\" width=\"800\" height=\"800\""<<endl; 
	out<<"fill=\"none\" stroke=\"black\" stroke-width=\"5px\" stroke-opacity=\"0.5\"/>"<<endl;
	
	//out<<"<image x=\"0\" y=\"0\" width=\"800px\" height=\"800px\""<<endl;
    //out<<"xlink:href=\"_112.jpg\">"<<endl;
	//out<<"</image>"<<endl;
	for(int i=0;i<vectorCount;i++){
		out<<"<text x=\""<<v[i].vCenter.x*n+d*n+3<<"\" y=\""<<v[i].vCenter.y*n+d*n<<"\" font-family=\"Verdana\" font-size=\"9\" fill=\"black\" >"<<i<<"</text>"<<endl;
		out<<"<line x1=\""<<v[i].vStart.x*n+d*n<<"\" y1=\""<<v[i].vStart.y*n+d*n<<"\" x2=\""<<v[i].vEnd.x*n+d*n<<"\" y2=\""<<v[i].vEnd.y*n+d*n<<
		"\" stroke-width=\"5\" stroke=\"green\" stroke-opacity=\"0.3\"/>"<<endl;
	}
	
	//out<<"<g stroke=\"blue\" >"<<endl;
	srand ( time(NULL) );
	for(int i=0;i<mask32Count;i++){
		if(!mask32[i].status)continue;
          out<<"<rect x=\""<<mask32[i].xMask*n+d*n<<"\" y=\""<<mask32[i].yMask*n+d*n<<"\" width=\""<<mask32[i].imgW*n<<"\" height=\""<<mask32[i].mH*n<<"\" "; 		
		out<<"fill=\"none\" stroke-width=\"2px\" stroke=\"rgb(";
		color= rand() % 100 + 1;
		out<<color<<"%,";
		color= rand() % 100 + 1;
		out<<color<<"%,";
		color= rand() % 100 + 1;
		out<<color<<"%)\"/>"<<endl;

	}
	//out<<"</g>";
	//out<<"<g fill-opacity=\"0.7\" stroke=\"black\" stroke-width=\"0.5px\">"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"red\"   transform=\"translate(  0,-50)\" />"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"blue\"  transform=\"translate( 70, 50)\" />"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"green\" transform=\"translate(-70, 50)\" />"<<endl;
	//out<<"</g>";
	out<<"</svg>";
	
	



    return out.str();
}


//________letterBase_____________	

letterBase::letterBase(void){
letterCount=0;
}


glyphOCR*  letterBase::operator[](int index){
	if (index<0 || index>=letterCount) {
		return &base[0];
	}
	return &base[index];
}//_______________________________________________	


void  letterBase::operator+=(glyphOCR ref){
	base[letterCount].copy(&ref); letterCount++;
}


int letterBase::indexById(unsigned int Id){	//находим нужную букву по idNumber
	int index=-1;
	for(int i=0;i<letterCount;i++){
		//cout<<i<<" idNumber="<<base[i].idNumber<<" Id="<<Id<<endl;
		if(base[i].idNumber==Id){
			index=i; 
			break;
		}
	}
	return index;
}	

