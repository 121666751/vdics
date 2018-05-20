/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZBASE_IMAGET_H__
#define __ZBASE_IMAGET_H__

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "mem.h"

#pragma warning(disable: 4267)

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#define   BAND_IMAGE  0
#define   RAW_IMAGE   1
#define   OLD_IMAGE   2
using namespace std;
using namespace boost;

class PointT;

typedef map<string,string> 		StrDict;
typedef vector<PointT> 			PointSeq;
typedef vector<char>			CharV;
typedef vector<unsigned char>	UcharV;
typedef vector<int>				IntV;
typedef vector<float>			FloatV;
typedef vector<double>			DoubleV;


/**
 ͼƬ�ļ���ʽ
 */
enum PicFormatT
{
	PIC_BMP	= 0x00000000,
	PIC_JPG	= 0x00000001,
};


/**
 * ָ���Զ�����ģ���࣬�����Զ��ͷ�ģ�����
 * ���磺
 	\code
 	PtrT<ScannerT> scanner = init_scanner();
 	scanner->scan();		// �����ScannerT*һ������PtrT<ScannerT>����
 	\endcode

 */
template<class T>
class PtrT
{
public:
	/** @param ptr ģ�����ָ�룬�ö������Ϊ��̬���룬��ͨ��free()�����ͷ� */
	PtrT(T* ptr):_ptr(ptr)
	{	if(!_ptr) throw_err("���󴴽�ʧ��");	}

	virtual ~PtrT() { if(_ptr) _ptr->free(); }

	/** ����->��������ʹ�ö�������󳣹�ָ��һ��������*/
	T* operator -> ()
	{
		if(!_ptr) throw_err("��ͼ���ʿ�ָ��");
		return _ptr;
	}

private:
	T* _ptr;
};

/**
 * Point type
 */
class PointT
{
public:
	/** Construct with existing X and Y coordinates, or default of 0, 0.*/
	PointT(	int x =0, int y =0 ):_x(x), _y(y)	{}

	/** Construct by copying coordinates from an existing PointT object.*/
	PointT(const PointT& pt) { _x = pt.x(); _y = pt.y(); }

	/** Virtual destructor does nothing.*/
	virtual ~PointT()		{;}


	/** Same as calling set() to set the coordinates of this object to
	those of an existing PointT object.*/
	void operator =(const PointT& pt)		{ _x = pt.x(); _y = pt.y(); }


	/** set the coordinates of this point to 0, 0.*/
	void	clear()		{ _x = _y = 0; }


	/** Offset the X coordinate of this point by distanceX, and the
	Y coordinate by distanceY. Either can be negative, which will subtract.*/
	void	offset(int distanceX, int distanceY)
	{
		_x += distanceX;
		_y += distanceY;
	}

	/** set the coordinates of this point to be the values x and y.*/
	void setPoint(int x, int y)		{	_x = x;	_y = y;  }

	/** get x */
	int	x()	const	{ return _x; }

	/** get y */
	int	y()	const	{ return _y; }

protected:
	/** Embedded Members.*/
	int _x;
	int _y;
};


/**
 * Region type
 */
class RectT
{
public:
	/** Construct with existing X, Y, and CX, CY coordinates, or default
	of 0, 0, 0, 0.*/
	RectT(int left = 0, int top = 0, int right = 0, int bottom = 0);
	

	/** Construct with existing RectT object.*/
	RectT(const RectT& r);

	/** Virtual destructor does nothing.*/
	virtual ~RectT()
		{;}


	/** Same as calling Set() to set the coordinates of this object
	to those of an existing RectT object.*/
	void operator =(const RectT& r)
		{ Set(r); }

	/** Return a PointT object which contains the coordinates of the
	left and top parts of this rectangle.*/
	PointT		topLeft() const;

	/** Return a PointT object which contains the coordinates of the
	right and top parts of this rectangle.*/
	PointT		topRight() const;

	/** Return a PointT object which contains the coordinates of the
	bottom and left parts of this rectangle.*/
	PointT		bottomLeft() const;

	/** Return a PointT object which contains the coordinates of the
	bottom and right parts of this rectangle.*/
	PointT		bottomRight() const;

	/** Return a PointT object which contains the coordinates of the
	center of this rectangle.*/
	PointT		GetCenterPoint() const;


	/** Return a reference to the left coordinate of this rectangle.*/
	int		left() const;

	/** Return a reference to the right coordinate of this rectangle.*/
	int		right() const;

	/** Return a reference to the top coordinate of this rectangle.*/
	int		top() const;

	/** Return a reference to the bottom coordinate of this rectangle.*/
	int		bottom() const;


	/** Return the height of this rectangle.*/
	int		height() const;

	/** Return the width of this rectangle.*/
	int		width() const;


	/** Returns true if an existing RectT object is completely inside
	this rectangle, false otherwise.*/
	bool		IsInside(RectT const& rcCompare) const;

	/** Returns true if the coordinates of this rectangle are 0,0,0,0.*/
	bool		IsNothing() const;

	/** Call to verify rectangle left and top coordinates are LESS than
	right and bottom coordinates.*/
	void		Normalize();

	/** Set the rectangle coordinates to those passed to function.*/
	void		Set(int left,int top,	int right,	int bottom );

	/** Set the coordinates of this rectangle to be the values from an
	existing RectT object.*/
	void		Set(RectT const& r);

protected:
	/** Embedded Members.*/
	int _left;
	int _top;
	int _right;
	int _bottom;
};

/**
 * ͨ��ͼ������, ���Դ洢1��3ͨ��ͼ��ͨ��ZIL�ӿڿ���ת��ΪZil Buffer�����
 * �Ӷ����и���ͼ�����������ɴ�Zil Buffer���ת���������Ӷ�ʵ���ڴ��Զ�
 * ����
 */
class ImageT
{
public:
	ImageT();
	/**
	 * �������캯��
	 */
	ImageT(const ImageT& rht);
	/**
	 * @param x:	ͼ����
	 * @param y:	ͼ��߶�
	 * @param bands:	ͼ��ͨ����������ȡֵΪ1��3
	 * @param dpi:		ɨ��ֱ��ʣ�����ȡֵΪ150,200,300
	 * @param data:		ͼ�����ݣ�ӦΪͨ��ͼ������Ϊ x * y * bands
	 */
	ImageT(int x, int y, int bands, int dpi, const MemT& data,int type=BAND_IMAGE);
	
	
	/**
	 * @param x:	ͼ����
	 * @param y:	ͼ��߶�
	 * @param bands:	ͼ��ͨ����������ȡֵΪ1��3
	 * @param dpi:		ɨ��ֱ��ʣ�����ȡֵΪ150,200,300
	 * @param data:		ͼ�����ݣ�ӦΪͨ��ͼ������Ϊ x * y * bands
	 */
	ImageT(int x, int y, int bands, int dpi, unsigned char* data,int type=BAND_IMAGE);

	/**  @return ͼ���� */
	int x() const ;

	/** @return ͼ��߶� */
	int y() const ;

	/** @return ͼ��ֱ��� */
	int dpi() const ;

	/** @return ͼ��ͨ���� */
	int bands() const;

	/** @return ͼ�����ݳ��� */
	int imglen() const ;
	/** @return ͼ���������� */
	int imgtype() const ;

	/**
	 * �б�ͼ���Ƿ���á�
	 * @return true-ͼ������Ϊ�գ���û��ͼ������,ͼ�񲻿��ã� false-ͼ�����
	 */
	bool empty() const;

	/** @return ͼ������,ͨ��ͼ */
	unsigned char* data() const ;

	/** @return ͼ�����ݣ�MemT��ʽ */
	const MemT& mem() const;
	

	/**
	 * ��תͼ�񣬽Ƕȱ���Ϊ90��������
	 * @param angle: ��ת�Ƕȣ�����Ϊ 90��180��270��
	 */
	void rotate(int angle);

	/**
	 * ͨ��������������ͼ��
	 * @param x:	ͼ����
	 * @param y:	ͼ��߶�
	 * @param bands:	ͼ��ͨ����������ȡֵΪ1��3
	 * @param dpi:		ɨ��ֱ��ʣ�����ȡֵΪ150,200,300
	 * @param data:		ͼ�����ݣ�ӦΪͨ��ͼ������Ϊ x * y * bands
	 */
	void setImage(int x, int y, int bands, int dpi, const MemT& data);

	void setImage(const ImageT& rht);

	void operator = (const ImageT& rht)
	{	setImage(rht.x(),rht.y(),rht.bands(),rht.dpi(),rht.mem()); setImgType(rht.imgtype());	}

	void setDpi(int dpi);
	
	void setImgType(int imgtype);


protected:

	/** �ڲ���ͨ����ת�㷨 */
	void rotateOneBand(int angle, unsigned char* srcImg, unsigned char* tarImg);
	
	int _sizeX;				/**< image size X */
	int _sizeY;				/**< image size Y */
	int _dpi;				/**< image dpi */
	int _bands;				/**< image pannels */
	MemT _data;				/**< image data. length=sizeX*sizeY*3 */
	int _imgtype;
	char reserved_image[252];	/**< not used by now */
};


ImageT mem_to_image(MemT m,int dpi);
ImageT subimage(const ImageT& fullImage, RectT& rc);

#endif // __ZBASE_IMAGET_H__
