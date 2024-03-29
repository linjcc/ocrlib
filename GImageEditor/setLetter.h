void GImageEditor::setLetter(commandData *inputData){

int print=1;
pechaImg=LoadImageData(inputData->data["inputFile"],0);
DT("inputData->data[\"inputFile\"]="<<inputData->data["inputFile"]<<END);
if(!pechaDataLoaded){cout<<"no image data"; return;}

xml_node col,data,resultSet,letterBaseXML, st;
xml_document doc;
int x0,x1,y0,y1,w,h;

unsigned int idNumber;	
string str;
glyphOCR *letter;
char *cbuf=(char*)malloc(inputData->data["setLetter"].size()+1);
memcpy(cbuf,inputData->data["setLetter"].c_str(),inputData->data["setLetter"].size());
cbuf[inputData->data["setLetter"].size()]=0;

if(!doc.parse(cbuf)){
		  cout<<"srcStr not loaded"<<END;return;
}

DT("setLetter="<<inputData->data["setLetter"]<<" cbuf="<<cbuf<<endl);
DT("inputData->data[\"newLetter\"]"<<inputData->data["newLetter"]<<END);
DT("aliKali.letterCount"<<aliKali.letterCount<<END);

///set line coordinates in page	
    GBitset *setBit=GBitset::createResize(pechaImg,1,1,1);
	DT("@4_2");
	vector<stringOCR>strArray; 
	setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )  
	DT("@4_3 strArray.size()="<<strArray.size()<<endl);
	setBit->destroy();
	
	
int index, letterFlag;
string name;
vector<glyphOCR>letterLine;
	

for (xml_node row = doc.child("glyph"); row; row = row.next_sibling("glyph")){

	index=-1; letterFlag=0;
	name=row.attribute("n").value();
   if(name=="*"||name=="")continue;
   //str=name.substr(0,name.find("/"));
/*	
  if(inputData->data["newLetter"]!=""){
	 for(int in=0; in<aliKali.letterCount; in++){
	 //	cout<<"aliKali[in]->name="<<aliKali[in]->name<<" name="<<name<<endl;
		if (aliKali[in]->name==name){cout<<"match";
	    glyphOCR letter_;	
	    letter_.copy(aliKali[in]); 

	    cout<<"new letter->name="<<letter_.name<<END;
	    int dl=4; //JavaScript compensation
		x0_new=atoi(row.attribute("x0").value());
		x1_new=atoi(row.attribute("x1").value())+dl;
		y0_new=atoi(row.attribute("y0").value());
		y1_new=atoi(row.attribute("y1").value())+dl;
	    idNumber=strtoul(row.attribute("l").value(),NULL,0); //cout<<" wp.idNumber="<<wp.idNumber<<endl;
	
		w_new=x1_new-x0_new; if(w_new>128)w_new=128;
		h_new=y1_new-y0_new; if(h_new>128)h_new=128;

		w=letter_.letterW;
		h=letter_.letterH;

		cout<<"x0="<<x0_new<<" y0="<<y0_new<<" x1="<<x1_new<<" y1="<<y1_new<<" w="<<w_new<<" h="<<h_new<<endl;
		letter_.lengthSum=0;
		letter_.newLetter=1;
		letter_.newLetterInBook=1;

	  GBitmap *img128;//=GBitmap::create(w_new,h_new);
	  //img128->fill(255);
			  if(idNumber){ 
				  int index=indexById(correctionTable,idNumber); cout<<"load from correctionTable index="<<index<<" idNumber="<<idNumber<<endl;
				  if(index){

					  cout<<"@@@ ind="<<index<<endl;
					  correctionTable[index].img->unpackImg(0);
					  
					  img128=GBitmap::createRegion(correctionTable[index].img, x0_new, y0_new, w_new, h_new);	 
					  cout<<"img128->columns()="<<img128->columns()<<endl;
					  
					  //LoadImageData(img128,correctionTable[indexById(correctionTable,idNumber)].img, x0_new,y0_new,w_new,h_new);
				  }else{cout<<"NO IMAGE IN BASE";return;}
			  }else{
				  img128=GBitmap::createRegion(pechaImg, x0_new, y0_new, w_new, h_new);		  
				  //LoadImageData(img128,pechaImg, x0_new,y0_new,w_new,h_new);			 
			  }
	          img128->printToScreen();
	      
			 DP("START_PACK"<<END);
			 letter_.mask128[0].packBitMask128(img128);
			 //glyphNew.mask128[letter->mask128Count].printMask();
			 //exit(0);
			 letter_.mask128[0].xMask=-letter_.mask128[0].mW/2;
			 letter_.mask128[0].yMask=-letter_.mask128[0].mH/2;
			 //letter->mask128[0].status=0;
			 //letter->mask128Count++;
	  cout<<"aliKali[in].name="<<aliKali[in]->name<<endl;
	  
	  glyphOCR glyph;	
	  glyph.copy(aliKali[in]); 
	  
	  int maxCorrelation=setLetterScale(img128, &glyph);
	  letter_.scale=glyph.scale;
	  
	  if(maxCorrelation>90){
		   for(int a=0;a<letter_.mask32Count;a++){
			   letter_.mask32[a].xMask=-1*(glyph.mask128[0].mW/2-glyph.mask32[a].xMask); //coordinate from center of mask128
			   letter_.mask32[a].yMask=-1*(glyph.mask128[0].mH/2-glyph.mask32[a].yMask);
			   cout<<"@mask m="<<a<<" xMask="<<letter_.mask32[a].xMask<<" yMask="<<letter_.mask32[a].yMask<<endl;
			   
		   }
		letter_.reloadMask();
		aliKali+=letter_;
		  
		   //if(inputData->data["newLetter"]=="true"){
			   letter=aliKali[aliKali.letterCount-1];
			   letter->recordFlag=1;
			   letter->name=name;
			   letter->letterIndex=aliKali.letterCount;
		   //}
	    letterFlag=1;	   
		break;   
	    	   
	   }	   
		
  }
}	
  }
	cout<<"@set new letter index="<<index<<" letterFlag"<<letterFlag<<endl;
*/
	
	if(!letterFlag){
		aliKali.letterCount++;
		letter=aliKali[aliKali.letterCount-1]; 
		letter->name=row.attribute("n").value();
		
		DT("letter->name="<<letter->name<<END);
		if(letter->name=="*"||letter->name==""){
		//cout<<"find"<<END;
		   continue;
		}
		int dl=4; //JavaScript compensation 
		x0=atoi(row.attribute("x0").value());
		x1=atoi(row.attribute("x1").value())+dl;
		y0=atoi(row.attribute("y0").value());
		y1=atoi(row.attribute("y1").value())+dl;
 	    idNumber=strtoul(row.attribute("l").value(),NULL,0); //cout<<" wp.idNumber="<<wp.idNumber<<endl;
        float scale;
		int textLineH;

		//set text line scale and coordinate
		for (int index=0;index<strArray.size(); index++){
			cout<<" strArray[index].LimY0="<<strArray[index].LimY0<<" strArray[index].LimY1="<<strArray[index].LimY1<<endl;
			if(y0>strArray[index].LimY0&&y0<strArray[index].LimY1){
				textLineH=strArray[index].y1-strArray[index].y0;
				//for(int a=0;a<inputData->lineSize.size();a++){  //set size of text line from page layout
				//	if(textLineH>inputData->lineSize[a]-inputData->lineSize[a]/5&&
				//	   textLineH<inputData->lineSize[a]+inputData->lineSize[a]/5){
				//		textLineH=inputData->lineSize[a];break;
				//	}
				//	
				//}
				cout<<" textLineH="<<textLineH<<endl;
                scale=(float)34/textLineH;
				textLineH=34;
				break;
			}
			
		}
	
		w=x1-x0; if(w*scale>128)w=128/scale;
		h=y1-y0; if(h*scale>128)h=128/scale;
		DT("x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<endl);
		letter->letterH=h*scale;  //SH(letter->letterH);
		letter->letterW=w*scale;  //SH(letter->letterW);
		letter->letterIndex=aliKali.letterCount-1;


		int flag=0; letter->stackFlag=1;
		 for(int d=0;d<uniTibTable.size();d++){
		    //cout<<"m11";
			if(uniTibTable.find(letter->name)!=uniTibTable.end()){
			   letter->Wylie=uniTibTable[letter->name].Wylie;
			   letter->stackFlag=uniTibTable[letter->name].stackFlag;
			   letter->OCRKey=uniTibTable[letter->name].OCRKey;
			   letter->OCRIndex=uniTibTable[letter->name].OCRIndex;
			   letter->baseLetter=uniTibTable[letter->name].baseLetter;
			   DT(" letter match ");	
			   flag=1; break;
			}
		 }
		letter->Character=0;
		letter->recordFlag=1;

		if(!flag){letter->Wylie=letter->name;}
		cout<<"m2 pechaImg->rows()="<< pechaImg->rows();
		GBitmap *img;
		if(idNumber){ 
			int index=indexById(correctionTable,idNumber); DT("load from correctionTable index="<<index<<" idNumber="<<idNumber<<endl);
			if(index){

				DT("@@@ ind="<<index<<endl);
				correctionTable[index].img->unpackImg(0);
			
				img=GBitmap::createRegion(correctionTable[index].img, x0, y0, w, h);	  
				//LoadImageData(img128,correctionTable[indexById(correctionTable,idNumber)].img, x0,y0,w,h);
			}else{cout<<"NO IMAGE IN BASE";return;}
		}else{
			img=GBitmap::createRegion(pechaImg, x0, y0, w, h);
			//LoadImageData(img128,pechaImg, x0,y0,w,h);			 
		}
		GBitmap *img128=GBitmap::createScale(img,scale);
		
		GBitsetContour *vSet=GBitsetContour::createResize(img128,1,1,0);
		vSet->textLineSize=textLineH;
		letter->textLineSize=textLineH;
		//cout<<"m3";
		vectorBase[0].allVectorCount=0;
		vSet->setFontData(vectorBase,letter,letter->name,0);
		//cout<<"vectorBase[0].allVectorCount="<<vectorBase[0].allVectorCount<<endl;
		vSet->destroy();
		
		 
		letter->maskFromVector();
		//string draw=letter->drawToSVG();
		//writeText(draw,"/1.svg");
		//img128->printToScreen();
		
		 
		DT("START_PACK img128.columns()="<<img128->columns()<<END);
			letter->mask128[0].packBitMask128(img128);  
			//letter->mask128[0].printMask();
			 //exit(0);
			 letter->mask128[0].xMask=-(letter->mask128[0].mW/2);
		     
		     //normalisation on slice size
		     int s= img128->columns(); if(s>128)s=128;
			 dl=letter->mask128[0].mW-s;
		     letter->xMin=500;
			 letter->xMax=0;
		     for(int a=0;a<letter->mask32Count;a++){
			     letter->mask32[a].xMask-=dl/2;
				 if(letter->mask32[a].xMask<letter->xMin)letter->xMin=letter->mask32[a].xMask;
				 if(letter->mask32[a].xMask+letter->mask32[a].imgW>letter->xMax)letter->xMax=letter->mask32[a].xMask+letter->mask32[a].imgW;
			}
		     //cout<<" letter->xMin="<<letter->xMin<<endl;
			 letter->dX=-(letter->mask128[0].mW/2-letter->letterW/2-(letter->mask128[0].mW/2+letter->xMin));
			 letter->dY=-(letter->mask128[0].mH/2-letter->letterH/2-(letter->mask128[0].mH/2+letter->yMin));

			 letter->mask128[0].yMask=-(letter->mask128[0].mH/2);
			 letter->mask128Count=1;
			 letter->idNumber=letter->mask128[0].hashValue();
		
		     letter->reloadMask();
		
		     //set text line coordinate
		//cout<<" y0="<<y0<<endl;
		for (int index=0;index<strArray.size(); index++){
			//cout<<"strArray[index].LimY0="<<strArray[index].LimY0<<" strArray[index].LimY1="<<strArray[index].LimY1<<endl;
			if(y0>strArray[index].LimY0&&y0<strArray[index].LimY1){
				letter->y0=(strArray[index].y0-y0)*scale;
				letter->y0=-(letter->mask128[0].mH/2-(letter->y0)*scale);
				letter->y1=(strArray[index].y1-y0)*scale;
				letter->y1-=letter->mask128[0].mH/2;
				break;
			}
		
		}
		
		 //cout<<"letter->mask128[0].xMask="<<letter->mask128[0].xMask<<endl;
		 //cout<<"if we don't find this letter in base we try to find it in letterbase store"<<endl;
		//setLetterFromStore(inputData,letter,img128); 
		img->destroy(); 
		img128->destroy();
		
		 
	}

}
//str=inputData->data["inputFolder"]+"/OCRLetterBase.xml";
//cout<<"str="<<str<<END;
writeLetterBase(inputData);

}//_____________________________________________________________________________


void GImageEditor::setBase(commandData *inputData){

xml_node col,data,resultSet,letterBaseXML, st;
xml_document doc;
int x0,x1,y0,y1,w,h,dlt0,letter_y0,letter_y1;
string str;
unsigned int index=0;	
int indexFlag=0;	
GBitmap *outBitmap;
string mode="set"; if(inputData->data["ocrData"]=="removeLetter")mode="remove";
//str=inputData->data["tablePath"]+"/OCRTables/OCRBase.jpg";
//LoadImageData(str,0);
	str=inputData->data["setBase"];
	
	cout<<"start set base mode="<<mode<<" str="<<str<<endl;
	str=decodeURLString(str);
	str=str_replace("{","<",str);
	str=str_replace("}",">",str);
	
char *cbuf=(char*)malloc(str.size()+1);
memcpy(cbuf,str.c_str(),str.size());

cbuf[str.size()]=0;
cout<<"inputData->data[\"setBase\"]="<<cbuf;

if(!doc.parse(cbuf)){
		  cout<<"setBase str not loaded"<<END;return;
}


for (xml_node row = doc.child("glyph"); row; row = row.next_sibling("glyph")){
	
	    index=strtoul(row.attribute("id").value(),NULL,0);
	    inputData->idNumber=index;
	    index=aliKali.indexById(index);	//находим нужную букву по idNumber	
		cout<<"idNumber="<<inputData->idNumber<<" name="<<aliKali[index]->name<<endl;
	    indexFlag=1;
	
	if(mode!="remove"){
		
		outBitmap=GBitmap::create(PICT_SIZE,PICT_SIZE);
		GBitmap *img=aliKali[index]->mask128[0].unPackMask(); 
		for (int y = 0; y <img[0].rows() ;y++){
			//pSrc = (unsigned char*)letterImage->Canvas->ScanLine[y];
			for (int x = 0; x <img[0].columns(); x++){
				
				//if((int)img[0][y][x]){cout<<"1";}else{cout<<"0";}
				
				if(img[0][y][x]){
					
				}else{
					//if(outBitmap[0]
					//   [y+aliKali[index]->mask128[0].yMask]
					//   [x+aliKali[index]->mask128[0].xMask]==255){
					outBitmap[0]
					[y+aliKali[index]->mask128[0].yMask+PICT_SIZE/2]
					[x+aliKali[index]->mask128[0].xMask+PICT_SIZE/2]=0;
					//}
				}
			}//cout<<END;
		}
		img->destroy();
	}
	    glyphOCR *letter=aliKali[index];
	    string name=row.attribute("n").value();
	    if(name!="")letter->name=name;
	    int n=atoi(row.attribute("sC").value());  //записываем данные о selfCorrelation и кодовом расстоянии
	    if(n)letter->selfCorrelation=n;
		n=atoi(row.attribute("cS").value());
	    if(n)letter->codeSpace=n;
	
		cout<<"letter->name="<<letter->name<<END;
		if(letter->name=="*"||letter->name==""||letter->name==" "){
		letter->recordFlag=0;  
		continue;
		}
		x0=atoi(row.attribute("x0").value());
		x1=atoi(row.attribute("x1").value());
		y0=atoi(row.attribute("y0").value());
		y1=atoi(row.attribute("y1").value());
		letter_y0=atoi(row.attribute("c0").value());
		letter_y1=atoi(row.attribute("c1").value());
		if(letter_y0!=0||letter_y1!=0){
			letter->y0=letter_y0-PICT_SIZE/2-((float)(letter_y0/PICT_SIZE))*PICT_SIZE;
			letter->y1=letter_y1-PICT_SIZE/2-((float)(letter_y1/PICT_SIZE))*PICT_SIZE;
		}


		if(x0!=0&&x1!=0&&y0!=0&&y1!=0){
		   w=x1-x0;
		   h=y1-y0;
		   letter->letterH=h;  //SH(letter->letterH);
		   letter->letterW=w;  //SH(letter->letterW);
		   letter->dX=x0-PICT_SIZE/2-((float)(x0/PICT_SIZE))*PICT_SIZE+w/2;
		   letter->dY=y0-PICT_SIZE/2-((float)(y0/PICT_SIZE))*PICT_SIZE+h/2;
		}
		letter->lengthSum=0;
		letter->newLetter=1;
		letter->newLetterInBook=1;

		int flag=0; letter->stackFlag=1;
		 for(int d=0;d<uniTibTable.size();d++){
		   //DM("d="<<d<<" "<<uniTibTable[d].Wylie.c_str()<<END);
			if(uniTibTable.find(letter->name)!=uniTibTable.end()){
			   letter->Wylie=uniTibTable[letter->name].Wylie;
			   letter->stackFlag=uniTibTable[letter->name].stackFlag;
			   letter->OCRKey=uniTibTable[letter->name].OCRKey;
			   letter->OCRIndex=uniTibTable[letter->name].OCRIndex;
			   letter->baseLetter=uniTibTable[letter->name].baseLetter;
			   flag=1; break;
			}
		 }
		letter->Character=0;
	    if(mode!="remove"){
		     letter->recordFlag=1;
		}else{
			letter->recordFlag=0;		
		}	

		if(!flag){letter->Wylie=letter->name;}
}

	if(mode=="remove")return;
    int maskCountOld=-1;

  for (xml_node row = doc.child("mask"); row; row = row.next_sibling("mask")){
	    if(!indexFlag){
			index=strtoul(row.attribute("g").value(),NULL,0);
			inputData->idNumber=index;
			//находим нужную букву по idNumber
			index=aliKali.indexById(index);
			indexFlag=1;
		}	
			//cout<<"index="<<index<<END;
		
		glyphOCR *letter=aliKali[index];
	    int maskId=atoi(row.attribute("id").value()); 
	    if(maskCountOld<0)maskCountOld=letter->mask32Count;
	
	    cout<<"add "<<letter->name<<" letter->mask32Count="<<letter->mask32Count<<" maskId="<<maskId<<" maskCountOld="<<maskCountOld<<END;
		
	    if(maskId>=maskCountOld)maskId=letter->mask32Count;
        if(maskId==letter->mask32Count)letter->mask32Count++;
	    cout<<"new letter->mask32Count="<<letter->mask32Count<<" maskId="<<maskId<<END;
		x0=atoi(row.attribute("x0").value());
		x1=atoi(row.attribute("x1").value());     //compensation of JavaScript
		y0=atoi(row.attribute("y0").value());
		y1=atoi(row.attribute("y1").value());
		dlt0=atoi(row.attribute("d").value());
		w=x1-x0;  if(w>32)w=32;
		h=y1-y0;  if(h>128)h=128;
		 //cout<<"x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<END;
		//letter->letterH=h;  //SH(letter->letterH);
		//letter->letterW=w;  //SH(letter->letterW);
		//letter->lengthSum=0;
		//letter->newLetter=1;
		//letter->newLetterInBook=1;
			 GBitmap *img32=GBitmap::createRegion(outBitmap,x0,y0,w,h);
			 //img32->fill(255);
			 //LoadImageData(img32,outBitmap,x0,y0,w,h);
			 letter->mask32[maskId].xMask=x0-PICT_SIZE/2;
			 //cout<<"index="<<index<<" index1="<<(index-((int)index/10)*PICT_SIZE)*PICT_SIZE<<END;
			 letter->mask32[maskId].yMask=y0-PICT_SIZE/2;
			 //OUT_C("mask.html");
				  letter->mask32[maskId].maskType=1;
				  letter->mask32[maskId].dlt0=dlt0;
				  letter->mask32[maskId].dlt1=1;
				  letter->mask32[maskId].packBitMask(img32);
	              cout<<"letter->mask32["<<maskId<<"].NMask="<<letter->mask32[maskId].NMask<<" letter->mask32["<<maskId<<"].yMask="<<letter->mask32[maskId].yMask<<END;
				  if(letter->mask32[maskId].NMask==0){letter->eraseMask(maskId); continue;}  //erase empty mask
			 //letter->mask32[id].printMask(inputData->c_out);
			 letter->mask32[maskId].status=1;
	         img32->destroy();
	         delete img32;
			 //out.close();
			 //str=readText("mask.html");
			 //cout<<"<div style=\"font-family:Courier;\">";
			 //cout<<str<<"</div>";exit(0);
	        
  } 
	 if(indexFlag){
	    glyphOCR *letter=aliKali[index];
		letter->xSum=0; letter->ySum=0;
		for(int maskId=0;maskId< letter->mask32Count;maskId++){
			letter->xSum+=letter->mask32[maskId].xMask;
			cout<<"letter->mask32["<<maskId<<"].xMask="<<letter->mask32[maskId].xMask<<END;
			letter->ySum+=letter->mask32[maskId].yMask;
		}
		 if(letter->xSum)letter->xSum/=letter->mask32Count;
		 if(letter->ySum)letter->ySum/=letter->mask32Count;
		 cout<<"letter->ySum="<<letter->ySum<<" letter->xSum"<<letter->xSum<<endl;
	 }
	 //test self correlation
	GBitmap *img128=aliKali[index]->mask128[0].unPackMask();
	glyphOCR glyph; glyph.copy(aliKali[index]);
	int c1=setCorrelation(img128,&glyph,1); aliKali[index]->selfCorrelation=c1;
	for(int a=0;a<glyph.mask32Count;a++){
		aliKali[index]->mask32[a].correlation=glyph.mask32[a].correlation;
	}
	
	img128->destroy();
	
	
	
//writeLetterBase(inputData->data["OCRBasePath"]);

}//_____________________________________________________________________________


void GImageEditor::drawLettersInCorrectionTable(commandData *inputData,int mode){
	///GBitmap *img;
	static int yImg=0,x0,y0,x1,y1;
	static int baseIndex=0;
	char cbuf[3];
	string path=fileName(inputData->data["inputFile"]);
	int w,h,print=1;
	cbuf[2]=0;
	int c;
	string str;
	
	if(mode==LETTER_ADD){
		for(int index=0;index<correctionTable.size();index++){
			//cout<<" path="<<path<<" correctionTable["<<index<<"].file="<<correctionTable[index].file<<" "<<END;
			//if(correctionTable[index].delimeter==" "&&correctionTable[index].name=="")continue;
			if(correctionTable[index].file!=path)continue;
			x0=correctionTable[index].x0-100;
			y0=correctionTable[index].y0-50;
			x1=correctionTable[index].x1+100;
			y1=correctionTable[index].y1+120;
			w=x1-x0; 
			  if(w%8<4){
					w-=w%8; //выравнивание битмапа по 8 увеличивает скорость обращения и необходимо для компрессии
			  }else{
					w-=w%8; 
			  }
				
			h=y1-y0;   
			DT("x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<endl);
			
			GBitmap *img=GBitmap::createRegion(pechaImg,x0,y0, w, h);  DT(" img->packFlag1="<<img->pack_flag);
			//LoadImageData(img,pechaImg,x0,y0, w, h); cout<<" img->packFlag2="<<img->pack_flag;
			correctionTable[index].img=GBitmap::create(img);
			correctionTable[index].imgFlag=1;
			img->destroy();
		}
				
		ostringstream out;
		string record;
		for(int index=0;index<correctionTable.size();index++){
			if(!correctionTable[index].imgFlag){cout<<"NO IMAGE"; continue;}
			out<<"<rec id=\""<<correctionTable[index].idNumber<<"\" f=\""<<correctionTable[index].file<<"\" ";
			out<<"n=\""<<correctionTable[index].name<<"\" c=\""<<correctionTable[index].correlation<<"\">";
			//int t=correctionTable[index].img->columns()*correctionTable[index].img->rows()/8;
			record="";
			str=correctionTable[index].img->drawToTIFFString(0);
			//str="34123514352346245634563456y345673567u4567u34567u";
			for(int m=0;m<str.size();m++) {
				c=(unsigned char)str[m];
				record+=charToHex(c);
				//record+="/";
				
				//*p=(unsigned int*)correctionTable[index].img->bytes_data+m;
				//in=*p;
				//sprintf(buff,"%X",in); record+=buff;record+=";";
				//if(correctionTable[index].img->bytes_data[m]>128){
				//   record+="0";
				//}else{
				//   record+="1";
				//}
				
			} 
			out<<"<img w=\""<<correctionTable[index].img->columns()<<"\" h=\""<<correctionTable[index].img->rows()<<"\">"<<record<<"</img></rec>\n";
			//break;
		}	
		
		/*
		while(!fileOpen(path)){
			sleep(1); //wait in line for writing
		}
		*/ 
		string str=out.str();
		//cout<<"STAT WRITE CORRECTION"<<endl;
		writeToLog(str,inputData->data["correctionTable"].c_str(),"/_1.txt");
		//cout<<"DONE WRITE CORRECTION"<<endl;
		
	}
}//_____________________________________________________________________________


