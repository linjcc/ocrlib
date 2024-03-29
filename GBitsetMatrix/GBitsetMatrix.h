﻿//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#ifndef _GBitsetMatrix_H_
#define _GBitsetMatrix_H_
#include "config.h"

#include "GBitmap/GBitmap.h"
#include "GBitmapInt/GBitmapInt.h"
#include "OCRTypes/OCRTypes.h"
#include "OCRTypes/glyphOCR.h"
#include "GBitMask/GBitMask.h"


#include <vector>
#include <bitset>
//#ifdef MAC
// #include <bvector.h>
//#endif
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>

using namespace std;


namespace ocr {

///класс матричного распознавания. Основной вычислительный движок корреляции букв из базы (letterBase *aliKali) и текста. 
	
class GBitsetMatrix{
protected:
  GBitsetMatrix(void);
  GBitsetMatrix(int nrows, int ncolumns, int border=0);
  GBitsetMatrix(const GBitsetMatrix *ref);
  GBitsetMatrix(const GBitmap *ref, short stepW, short stepH,short flipV=0);
  GBitsetMatrix(const GBitmap *ref, short x, short y, short w, short h, short stepW, short stepH, short flipV=0);
  GBitsetMatrix(const GBitsetMatrix *ref, int border);
public:
  virtual ~GBitsetMatrix();
  void destroy(void);
  /** @name Construction. */
  //@{
  /** \brief Constructs an empty GBitsetMatrix object.  The returned GBitsetMatrix has zero rows and zero columns. Use function \Ref{init} to change the size of the image. */
  static GBitsetMatrix* create(void) {return new GBitsetMatrix;}

  /** \brief Constructs a GBitsetMatrix with #nrows# rows and #ncolumns# columns.  All pixels are initialized to white. */
  static GBitsetMatrix* create(const int nrows, const int ncolumns, const int border=0)
  {return new GBitsetMatrix(nrows,ncolumns, border); }

  /** \brief Copy constructor. Constructs a GBitsetMatrix by replicating the size and the contents of GBitsetMatrix #ref#. */
  static GBitsetMatrix* create(const GBitsetMatrix *ref)
  {return new GBitsetMatrix(ref);}

  /** \brief Copy constructor. Constructs a GBitsetMatrix by replicating the size and the contents of GBitmap #ref#. */
  static GBitsetMatrix* createResize(const GBitmap *ref, short stepW, short stepH, short flipV)
  {return new GBitsetMatrix(ref,stepW,stepH,flipV);}

   /** \brief Copy constructor. Constructs a GBitsetMatrix by region of the contents of GBitmap #ref#. */
  static GBitsetMatrix* createRegion(const GBitmap *ref, short x, short y, short w, short h, short stepW, short stepH, short flipV)
  {return new GBitsetMatrix(ref,x,y,w,h,stepW,stepH,flipV);}


  /** \brief Constructs a GBitsetMatrix by copying the contents of GBitsetMatrix #ref#.*/
  static GBitsetMatrix* create(const GBitsetMatrix *ref, const int border)
  { return new GBitsetMatrix(ref,border); }
  //@}
   /** @name Initialization. */
  //@{
      /** \brief Resets this GBitsetMatrix size to #nrows# rows and #ncolumns# columns and sets all pixels to white. */ 
  void init(int nrows, int ncolumns, int border);
      /** \brief Initializes this GBitsetMatrix with the contents of the GBitsetMatrix #ref#. */
  void init(const GBitsetMatrix *ref, int border);
  /** \brief Initializes this GBitsetMatrix with the contents of the GBitsetMatrix #ref#. stepH, stepW is step for reduce image size */
  void init(const GBitmap *ref, short stepW, short stepH, short flipV);
  /** \brief  Initializes this GBitsetMatrix with a rectangular segment #rect# of GBitmap #ref#. */
  void init(const GBitmap *ref, short x, short y, short w, short h, short stepW, short stepH, short flipV);

  GBitsetMatrix* operator=(const GBitsetMatrix *ref);
  /** \bref Initializes all the GBitsetMatrix pixels to value #value#. */
  void fill(bool value);
  //@}

  /** @name Accessing the pixels and dimention data. */
  //@{
  /** \brief Returns the number of rows (the image height). */
  unsigned int rows() const;
  /** \brief Returns the number of columns (the image width). */
  unsigned int columns() const;
  /** \brief Returns the size the border */
  unsigned int borderSize() const;
  /** \brief Returns a constant pointer to the N byte of row #row#.This pointer can be used as an array to read the columns elements. */
  const bool *operator[] (int row) const;
  /** \brief Returns a pointer to the first byte of row #row#.This pointer can be used as an array to read or write the row elements. */
  bool *operator[] (int row);
  /** \brief Returns the size of a row in memory (in pixels). */
  unsigned int rowsize() const;
  /* \brief Копирует маску во внутренний массив увеличивает счетчик массива (deprecated)*/
  void setMask(GBitMask32 *srcMask, int maskSize);

  //@}

protected:
  unsigned short nrows;         ///<высота битмэпа
  unsigned short ncolumns;      ///<ширина битмэпа
  unsigned short border;        ///<размер бордюра
  unsigned short bits_per_row;  ///<количество битов в строке
  unsigned int *srcInt;         ///<буфер массива горизонтальных строк
  unsigned int *sInt;           ///<буфер транспонированого по вертикали массива горизонтальных строк
  short stepBit;
  short srcW;
  short srcH;

public:
  bool  *bits_data;               ///<буфер данных битмепа в байтах
  unsigned char cacheCor[4352];   ///< массив кэша коэффициента корреляции по области поиска максимума 128*34
  unsigned int maskCor[384*32*4]; ///< кэш масок ON  и  OFF  (setMatrix_Lion)
  int data_size;                  ///<количество пикселов в битмэпе
  #ifdef DEBUGLVL
	ofstream c_out;               ///<вывод в отладочный файл
  #ifdef MAC
	drawDataRecord drawGraph;     ///<отладочный графический массив

  #endif
#endif

public:

//float Alpha;
  short deltaX;
  short lineCount;               ///<общее количество строк на странице
  int stringNum;                 ///<номер строки в тексте
#ifdef DEBUGLVL_GREY								
  GBitmap* drawData;             ///<отладочный графический массив
#endif
#ifdef DEBUGLVL_RGB								
  GBitmapInt* drawDataRGB;       ///<отладочный графический массив
#endif
  short baseDirection;            ///<направление письма в языке
  vector<stringOCR>*strArray;     ///<массив координат строк и букв
  glyphOCR *aliKali;              ///<ссылка на базу букв 
  vector<frameOCR>frameArray;     ///<массив координат блоков букв в строке
  GBitMask32 *mask32[32];         ///массив масок 32х32
  int mask32Count;                ///<размерность массива масок 32х32



/*/ --------------------------------------------------------
// --------------------------------------------------------
// --- Matrix function---GBitsetMatrix_Starting.h ---


 /// Запуск подпрограммы управления матрицы и установление начальных прараметров
  void Matrix_Starting(vector<stringOCR>&strArray_,glyphOCR *aliKali_,int stringNum_);
 */

 /// основная функция матричной корреляции букв базы и текста
  void letterCorrelation ( commandData *inData,
							vector<OCRMatch>&matchLine,
							letterBase *aliKali,
							int y0,
							int y1,
						    int limit,
							int print);
  ///тестовая функция вычисления корреляции одной буквы из базы
  int letterToLetterCorrelation(glyphOCR *glyph,
						    int y0,
						    int y1,
							int print);
  ///вычисление велицины кореляции буквы из базы по заданным координатам C++ версия	
  int  setMatrixCorrelation(GBitMask32 *mask32,
							int xSrc0,
							int ySrc0,
							int xSrc1,
							int ySrc1,
							int dlt,
							int *maxX,
							int *maxY,
							ofstream &c_out);
	
  ///вычисление величины корреляции буквы из базы по заданным координатам ASM версия	
 int setMatrixCorrelationASM(GBitMask32 *mask32,
							int xSrc0,
							int ySrc0,
							int xSrc1,
							int ySrc1,
							int dlt,
							int *maxX,
							int *maxY,
							ofstream &c_out);
 ///вычисление величины корреляции признака буквы из базы по заданным координатам прямоугольника методом "Льва в пустыне" 	
 int setMatrix_Lion(GBitMask32 *mask32,
							int xSrc0,
							int ySrc0,
							int xSrc1,
							int ySrc1,
							int dlt,
							int *maxX,
							int *maxY,
							ofstream &c_out,
							int print);
  ///вычисление величины корреляции признака буквы из базы по заданным координатам 		
  int setMatrix_Point(GBitMask32 *mask32,
							int xP,
							int yP,
							int sx0,
							int sy0,
							int w,
							int *ch,
							ofstream &c_out,
							int print);
  ///вычисление величины корреляции признака буквы из базы по заданным координатам  (тест)	
  int setMatrix_PointT(GBitMask32 *mask32,
							int xP,
							int yP,
							int sx0,
							int sy0,
							int w,
							int *ch,
							ofstream &c_out,
							int print);


};




// --------------------------------------------------------

// ---------------- IMPLEMENTATION

inline unsigned int
GBitsetMatrix::rows() const
{
  return nrows;
}

inline unsigned int
GBitsetMatrix::columns() const
{
  return ncolumns;
}


inline unsigned int
GBitsetMatrix::borderSize() const
{
  return border;
}


inline unsigned int
GBitsetMatrix::rowsize() const
{
  return bits_per_row;
}


inline bool*
GBitsetMatrix::operator[](int row)
{
  if (row<0 || row>=nrows) {
	return  bits_data;
  }
  return &bits_data[row * bits_per_row + border];
}

inline const bool*
GBitsetMatrix::operator[](int row) const
{
   if (row<0 || row>=nrows) {
	return bits_data;
  }
  return &bits_data[row * bits_per_row + border];
}

inline GBitsetMatrix*
GBitsetMatrix::operator=(const GBitsetMatrix*ref)
{
  init(ref, ref->border);
  return this;
}
// ---------------- THE END

}//namespace

#endif


