﻿/** @fn void GImageEditor::drawString(int nStr, int border)
 @brief вывод в стандартный выходной поток HTML отображения
 глобального массива результатов распознования strArray
 параметры вывода задаются в  управляющей структуре inputData
 x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
 x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
 */
void GImageEditor::drawStrArray(commandData *inputData,vector<stringOCR>&strArray, int border){
	
	int strInd,m;
	int x0,x1,y0,y1;        	
	int pageWide=0;
	if(pechaImg->columns()>pechaImg->rows())pageWide=1;
	string fontName=aliKali.fontName;
	if(fontName=="")fontName="Kailasa";
	
	cout<<"@@@draw pageWide="<<pageWide<<" pechaImg->columns()="<<pechaImg->columns()<<" pechaImg->rows()="<<pechaImg->rows()<<endl;
	
	string path=fileName(inputData->data["inputFile"]);
	//cout<<"inputData->data[\"inputFile\"]="<<inputData->data["inputFile"];
	
	#define c_out1   inputData->c_out
	
	path=inputData->data["root"]+"/edit/headerPage.xml";
	string headerPageHTML=readText(path);
	cout<<"path@@="<<path<<END;
	
	c_out1<<headerPageHTML<<endl;
	c_out1<<"			var pictW = "<<pechaImg[0].columns()<<";"<<endl;
	
	
	for ( strInd=strArray.size()-1; strInd >=0; strInd-- ){
	    for ( m=0; m < strArray[strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[strInd].wordArray[m];
			if(wP->id<0)continue;
			x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
			c_out1<<"   frame["<<wP->id<<"]=new Array(4); ";
			c_out1<<"frame["<<wP->id<<"].x="<<x0<<"; ";
			c_out1<<"frame["<<wP->id<<"].y="<<y0<<"; ";
			c_out1<<"frame["<<wP->id<<"].w="<<x1-x0<<"; ";
			c_out1<<"frame["<<wP->id<<"].h="<<y1-y0<<";"<<endl;
		}
	}
	cout<<"@@@"<<strInd<<endl;
	if(pageWide){
		c_out1<<"function a(index){"<<endl;
		c_out1<<"	//alert(document.getElementById(\"frame\"));"<<endl;
		c_out1<<"	var xFrame=frame[index].x*(1600/pictW*3)+20;"<<endl;
		c_out1<<"	var yFrame=frame[index].y*(1600/pictW*3)+20;"<<endl;
		//c_out1<<"	//alert(xFrame);"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.left=xFrame+\"px\";"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.top=yFrame+\"px\"; "<<endl;
		//c_out1<<"	document.getElementById('outputStr').innerHTML=frame[index].x+":|:"+frame[index].y;"<<endl;
		c_out1<<"}\n";
	}else{
		c_out1<<"function a(index){"<<endl;
		c_out1<<"	//alert(document.getElementById(\"frame\"));"<<endl;
		c_out1<<"	var xFrame=frame[index].x*(600/pictW)+640-10;"<<endl;
		c_out1<<"	var yFrame=frame[index].y*(600/pictW)-10;"<<endl;
		//c_out1<<"	//alert(xFrame);"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.left=xFrame+\"px\";"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.top=yFrame+\"px\"; "<<endl;
		//c_out1<<"	document.getElementById('outputStr').innerHTML=frame[index].x+":|:"+frame[index].y;"<<endl;
		c_out1<<"}\n";
	}	
	c_out1<<"var pageLink=\""<<fileName(inputData->data["inputFile"])<<"\";\n";
	
	c_out1<<"</script>";
	c_out1<<"</head><body topmargin=\"0px\" leftmargin=\"0px\" onLoad=\"link()\">"<<endl;
	c_out1<<"<div id=\"frame\" style=\"width:30px; height:30px; position:absolute; border:2px solid red\"></div>";
	
	if(pageWide){
		c_out1<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;";
		c_out1<<"text-align: left; width: 1600px; left:20px; z-index:-1; top: 20px;\"><br />"<<endl;
		c_out1<<"<img id=\"pechaImg\" src=\"\" width=\"1600px\">"<<endl;
		c_out1<<"</div>";
		
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width:1600px; position:absolute; top:350px; left:250px;\"><br>"<<END;
		c_out1<<mainString;
		c_out1<<"</div>";
		
	}else{
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width: 600px; position:absolute; top:20px; left:20px;\"><br>"<<END;
		c_out1<<mainString;
		c_out1<<"</div>";
		c_out1<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;";
		c_out1<<"text-align: left; width: 480px; left:640px; z-index:-1; top: -20px;\"><br />"<<endl;
		c_out1<<"<img id=\"pechaImg\" src=\"\" width=\"600px\">"<<endl;
		c_out1<<"</div>";
		
	}	
	c_out1<<"<div id=\"page_form\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;top:900px;\">";
	c_out1<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"/cgi/yagpoOCR.cgi\" name=\"main\">\n";
	c_out1<<"<input  name=\"inputFile\"  id=\"inputFile\" type=\"hidden\" value=\""; c_out1<<inputData->data["inputFile"];c_out1<<"\"/>\n";
	c_out1<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/>\n";
	c_out1<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; c_out1<<inputData->data["inputFolder"];c_out1<<"\"/>\n";
//	c_out1<<"<button name=\"submit\" value=\"submit\" type=\"submit\">Set letter in base</button>";
//	c_out1<<"<input  name=\"newLetter\"  type=\"checkbox\" value=\"true\" />Set as new letter<br>\n";
	c_out1<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"70\" rows=\"3\"></textarea>\n";
	c_out1<<"</form></div>";
	
	for(int i=0;i<25;i++){
		c_out1<<"<div class=\"dE\" id=\"g"<<i<<"\" c=\"83\" style=\"left:40px; top:0px; width:30px; height:30px; \">"<<endl;
		c_out1<<"<div class=\"dM\" style=\"width: 17px; height:3px;\"></div>"<<endl;
		c_out1<<"<div class=\"nG\" id=\""<<i<<"\" style=\" top:39px\" onClick=\"edit("<<i<<")\" onBlur=\"setText("<<i<<")\">*</div></div>"<<endl;
	}
	
	path=inputData->data["inputFolder"]+"/_all_text_html.html";
    //ofstream c_out; c_out.open(path.c_str(),ofstream::app);
    //c_out<<"{"<<inputData->data["inputFile"]<<"}"<<endl;
	//c_out<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width: 600px;\"><br>"<<END;
    //c_out<<mainString;
	//c_out<<"</div>\n";
	//c_out.close();
	
#undef c_out1
	inputData->c_out.close();
	
	// x0,y0,x1,y1
	
}//_____________________________________________________________________________


/** @fn void GImageEditor::drawString(int nStr, int border)
 @brief вывод в стандартный выходной поток HTML отображения
 глобального массива результатов распознования strArray
 
 параметры вывода задаются в  управляющей структуре inputData
 */
void GImageEditor::drawStrArrayDebug(commandData *inputData,vector<stringOCR>&strArray, int border){
	
	int strInd,m,strFlag;
	int x,x0,x1,y0,y1,gY;        // ,xt0,yt0,xt1,yt1
	//int MaxString=60; // MaxString - ограничение на колличество выводимых на эран строк
	//char cbuf[10];
	// x0,x1, y0,y1     координаты квадрата вокруг буквы
	int index=0;
	int print=0;
#define c_out1   inputData->c_out

	string fontName=aliKali.fontName;
	if(fontName=="")fontName="Kailasa";

	string path=inputData->data["root"]+"edit/headerPageLetter.xml";
	replace(path.begin(),path.end(),'\\','/');
	cout<<"pathDRAW="<<path<<END;
	string headerPageHTML=readText(path);

	c_out1<<headerPageHTML<<endl;

	c_out1<<"</script>"<<endl;
	c_out1<<"<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">"<<endl;
	c_out1<<"</head>"<<endl;
	c_out1<<"<body topmargin=\"0px\" leftmargin=\"0px\">"<<endl;
	c_out1<<"<style>";
	c_out1<<".nG {font-family:"<<fontName<<"}"<<endl;
	c_out1<<".nT {font-family:"<<fontName<<"}"<<endl;
	c_out1<<".eG {font-family:"<<fontName<<"}"<<endl;
	c_out1<<"</style>";

	path=fileName(inputData->data["inputFile"]);
	cout<<"path="<<inputData->data["inputFileName"]<<END;
	//cout<<"inputData->data[\"inputFile\"]="<<inputData->data["inputFile"];
	
	c_out1<<"<div style=\"position:absolute; left:0px; top:0px;\">";
	c_out1<<"<img class=\"srcImg\" src=\""<<path<<"\"/>\n";
		
	// заполнение массива координат прямоугольника
	for ( strInd=strArray.size()-1; strInd >=0; strInd-- ){ 
		strFlag=strArray[strInd].stringFlag; 
		x0=strArray[strInd].x0;   x1=strArray[strInd].x1;
		y0=strArray[strInd].y0;   y1=strArray[strInd].y1;
		c_out1<<"<div class=\"sT\" id=\"s"<<strInd<<"\" style=\"left:";
		c_out1<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
		c_out1<<y1-y0<<"px;\" ";
		c_out1<<"LimY0=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"LimY1=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"y0=\""<<strArray[strInd].y0<<"\" ";
		c_out1<<"y1=\""<<strArray[strInd].y1<<"\" ";
		c_out1<<"x1=\""<<strArray[strInd].x1<<"\" ></div>\n";
		
		if(strFlag<1)continue; // если stringFlag==0 то строку не заполняем квадратиками букв (в строке линия)
		for ( m=0; m < strArray[strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[strInd].wordArray[m];
			if(wP->correlation<0)continue;  //word marked as text frame etc. 
			x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
			gY=strArray[strInd].y1-y0;
			DT("y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" LimY0="<<strArray[strInd].LimY0<<" LimY1="<<strArray[strInd].LimY1<<END);
			if(x1-x0>0){
				c_out1<<"<div class=\"dE\" id=\"g"<<index<<"\" c=\""<<wP->correlation;
				c_out1<<"\" style=\"left:"<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
				c_out1<<y1-y0<<"px; ";
				if(wP->correlation<70){c_out1<<" border-color:#ff0000; border-width:3px;";}
				if(wP->correlation>70&&wP->correlation<80){c_out1<<" border-color:#cc0033; border-width:2px;";}
				
				c_out1<<"\">\n<div class=\"dM\" style=\"width: "<<x1-x0<<"px; height:3px;\"></div>\n";
				//c_out1<<wP->OCRIndex;
				c_out1<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:"<<gY<<"px\" onClick=\"edit(";
				c_out1<<index<<")\" onBlur=\"setText("<<index<<")\">"<<wP->name<<"</div></div>";
				index++;
			}
			//DT("n="<<wP->name.c_str()<<" c="<<wP->correlation<<END);
			//if(wP->stackFlag>0){
			//ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии салатовый
			//}else{
			//ImageEnView1->ObjPenColor[in]=(TColor)0xff1400; // цвет линии светло синий
			//}
			
		} // m
		
	} // strInd
	
	
	c_out1<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"/cgi/yagpoOCR.cgi\" name=\"main\">\n";
	c_out1<<"<input  name=\"inputFile\" id=\"inputFile\" type=\"hidden\" value=\""; c_out1<<inputData->data["inputFile"];c_out1<<"\"/><br/>\n";
	c_out1<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/><br/>\n";
	c_out1<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; c_out1<<inputData->data["inputFolder"];c_out1<<"\"/><br/>\n";
//	c_out1<<"<button name=\"submit\" value=\"submit\" type=\"submit\">Set letter in base</button><br/>\n";
//	c_out1<<"<input  name=\"newLetter\"  type=\"checkbox\" value=\"true\" /><p>Set as new letter</p>\n";
	c_out1<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"110\" rows=\"7\"></textarea>\n";
	c_out1<<"</form>";
	//string t;
	//t=str_replace("\n","</div><br/><div class=\"nT\">",mainString);
	//c_out1<<"<div class=\"nT\">"<<mainString<<"</div>";
	c_out1<<"</div></body>";
	
#undef c_out1
	
	//ofstream out_st;  out_st.open("/_out.html");
	//out_st<<"<html><head><meta content=\"text/html; charset=UTF-8\" http-equiv=\"Content-Type\"></head><body>";
	//out_st<<"<div style=\"font-family:"<<fontName<<"; font-size:2em;\";>"<<END;
    //out_st<<mainString;
	//out_st<<"</div>";
	//out_st.close();
	
	//SH("done");
	//print=0;
	// x0,y0,x1,y1
	// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
	// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
	
	
}//_____________________________________________________________________________

string GImageEditor::drawLetterBlock(commandData *inputData,unsigned int idNumber){
	//cout<<"draw letter"<<endl;
	string str;
	int in=-1;
	cout<<"input id="<<idNumber<<endl;
	//находим нужную букву по idNumber
	for(int i=0;i<correctionTable.size();i++){
		//cout<<"correctionTable["<<i<<"].idNumber="<<correctionTable[i].idNumber<<endl;
		if(correctionTable[i].idNumber==idNumber){
			in=i; break;
		}
	}
	cout<<"INDEX="<<in<<" TIFF_string_size="<<correctionTable[in].img->TIFF_string_size<<endl;
	if(in<0)return "";
	//correctionTable[in].img->unpackImg();
	//cout<<"done draw";
	str=correctionTable[in].img->drawToTIFFString(0);
	
	return str;
}





















