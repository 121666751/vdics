/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#pragma once

#include <iostream>
#include <cstdlib>

#include "ZTSExcept.h"

extern "C" {
#include <libjpeg/jpeglib.h>
}

#define jpeg_COMPRESS_BUFFER_SIZE 4096
#define jpeg_DECOMPRESS_BUFFER_SIZE 4096


namespace jpeg {

	/**
	* Inherit from this class to allow jpeg decompression from data.
	*/ 
	class IStream {
	public:
		IStream(void) {};
		virtual ~IStream(void) {};

		/**
		* Tries to read nb_to_read bytes, and stores them in a large
		* enough byte buffer dest_buf.
		*/
		virtual void readBytes(char* dest_buf,int nb_to_read)=0;

		/**
		* Returns the number of bytes actually read by last readBytes call.
		*/
		virtual int countReadBytes(void)=0;
	};

	/**
	* This allow the management of std::istream object.
	*/
	class IStdStream : public IStream {
	private :

		std::istream* input; 

	public:

		IStdStream(void) : IStream(), input(NULL) {};
		virtual ~IStdStream(void) {};

		void setStream(std::istream& is) {input = &is;} 
		virtual void readBytes(char* dest_buf,int nb_to_read) {
			input->read(dest_buf,nb_to_read);
		}

		virtual int countReadBytes(void) {
			return (int)(input->gcount());
		}
	};

	/**
	* Inherit from this class to allow jpeg decompression from data.
	*/ 
	class OStream {
	public:
		OStream(void) {};
		virtual ~OStream(void) {};

		/**
		* writes nb_to_read bytes.
		*/
		virtual void writeBytes(char* src_buf,int nb_to_write)=0;
	};

	/**
	* This allow the management of std::istream object.
	*/
	class OStdStream : public OStream {
	private :

		std::ostream* output; 

	public:

		OStdStream(void) : OStream(), output(NULL) {};
		virtual ~OStdStream(void) {};

		void setStream(std::ostream& os) {output = &os;} 


		virtual void writeBytes(char* src_buf,int nb_to_write) {
			output->write(src_buf,nb_to_write);
		}

	};


	class Compress { 

	public:

	private: 

		OStream* output; 
		OStdStream std_output;

		JOCTET jpg_buf[jpeg_COMPRESS_BUFFER_SIZE];
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		struct jpeg_destination_mgr mout;

		static void init_destination(j_compress_ptr cinfo) {
			cinfo->dest->next_output_byte
				=(JOCTET*)(((jpeg::Compress*)(cinfo->client_data))->jpg_buf); 
			cinfo->dest->free_in_buffer=jpeg_COMPRESS_BUFFER_SIZE;
		}

		static boolean empty_output_buffer (j_compress_ptr cinfo) {
			jpeg::Compress* that = (jpeg::Compress*)(cinfo->client_data); 
			that->output->writeBytes((char*)(that->jpg_buf),jpeg_COMPRESS_BUFFER_SIZE);
			init_destination(cinfo);
			return TRUE;
		}

		/*static void term_destination(j_compress_ptr cinfo) {
			jpeg::Compress* that = (jpeg::Compress*)(cinfo->client_data); 
			that->output->writeBytes((char*)(that->jpg_buf),
				(size_t)(
				(int)(cinfo->dest->next_output_byte)
				- (int)(that->jpg_buf)));
		}*/

	public:

	/*	Compress(void) {
			output = (OStream*)NULL;
			cinfo.err=jpeg_std_error(&jerr);
			jpeg_create_compress(&cinfo);

			mout.init_destination=jpeg::Compress::init_destination;
			mout.empty_output_buffer=jpeg::Compress::empty_output_buffer;
			mout.term_destination=jpeg::Compress::term_destination;
			cinfo.dest=&mout;
			cinfo.client_data = (void*)this;
		};

		~Compress(void) {
			jpeg_destroy_compress(&cinfo);
		}
*/
		/**
		* Set the output stream the jpeg file is written in. It has to be
		* correctly opened.
		*/
		void setOutputStream(std::ostream& os) { 

			std_output.setStream(os);
			output = &std_output; 
		}  

		/**
		* Set the output stream the jpeg file is written in. It just need
		* to be inherited from jpeg::Decompress::Stream interface. 
		*/
		void setOutputStream(OStream* os) {
			output = os;
		}

		/**
		* This writes the jpeg file.
		* @param width image width.
		* @param height image height.
		* @param depth 3 for color, 1 for grayscale.
		* @param dpi	resolution of the image. eg 150,200,300
		* @param img_buf the rgbrgbrgb.... or gggggggg.... data line by line(according to depth).
		* @param quality in [1-100]
		*/
		void writeImage(int width, int height, int depth, int dpi,
			unsigned char* img_buf,int quality=75) {

				unsigned char* row;                 /* Store adress of the first element of the current row */
				size_t row_stride;                  /* depth*width */


				cinfo.image_width=width;
				cinfo.image_height=height;
				cinfo.input_components=depth;
				cinfo.X_density = dpi;
				cinfo.Y_density = dpi;
				if(depth==1)
					cinfo.in_color_space=JCS_GRAYSCALE;
				else
					cinfo.in_color_space=JCS_RGB;
				jpeg_set_defaults(&cinfo);
				jpeg_set_quality(&cinfo,quality,TRUE);

				row_stride=width*depth;
				jpeg_start_compress(&cinfo,TRUE);
				while(cinfo.next_scanline<cinfo.image_height)
				{
					row=img_buf+(cinfo.next_scanline*row_stride);
					(void) jpeg_write_scanlines(&cinfo,&row,1);
				}
				jpeg_finish_compress(&cinfo);
			}
	};

	class Decompress { 

	private: 

		IStream* input; 
		IStdStream std_input;
		OStream* jpeg_copy;
		OStdStream std_jpeg_copy;

		struct my_error_mgr {
			struct jpeg_error_mgr pub;
		};

		struct my_src_mgr {
			struct jpeg_source_mgr pub; 
			jpeg::Decompress* that;
		};

		struct my_error_mgr jerr;
		struct my_src_mgr src;
		struct jpeg_decompress_struct cinfo;
		JOCTET jpg_buf[jpeg_DECOMPRESS_BUFFER_SIZE];
		int row_stride;

		void CopyBuffer(void) {
			if(jpeg_copy!=NULL)
				jpeg_copy->writeBytes((char*)(src.pub.next_input_byte),
				src.pub.bytes_in_buffer);

		}

		static void init_source(j_decompress_ptr cinfo) {
			((struct my_src_mgr*)(cinfo->src))->that->initSource();
		}

		void initSource(void) {
			CopyBuffer();
		}

		static boolean fill_input_buffer(j_decompress_ptr cinfo) {
			return ((struct my_src_mgr*)(cinfo->src))->that->fillInputBuffer();
		}

		boolean fillInputBuffer(void) {
			input->readBytes((char*)jpg_buf,jpeg_DECOMPRESS_BUFFER_SIZE);
			src.pub.next_input_byte = jpg_buf;
			src.pub.bytes_in_buffer = input->countReadBytes();

			CopyBuffer();

			return TRUE;
		}

		static void skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
			((struct my_src_mgr*)(cinfo->src))->that->skipInputData(num_bytes);
		}

		void skipInputData(long num_bytes) {

			if (num_bytes > 0) {
				while (num_bytes > (long) src.pub.bytes_in_buffer) {
					num_bytes -= (long) src.pub.bytes_in_buffer;
					(void) fillInputBuffer();
				}
				src.pub.next_input_byte += (size_t) num_bytes;
				src.pub.bytes_in_buffer -= (size_t) num_bytes;
			}
		}

		static void term_source(j_decompress_ptr cinfo) {
		}

		static void on_exit (j_common_ptr cinfo) {
			std::cerr << "JPG图像文件已经损坏!" << std::endl;
			throw_err("JPG图像文件已经损坏!");
		}

		
	private:

		void InitJPEG(void) {
			jerr.pub.error_exit = on_exit;

			jpeg_create_decompress(&cinfo);
			cinfo.err = jpeg_std_error(&jerr.pub);
			cinfo.err->error_exit = on_exit;

			cinfo.src = (jpeg_source_mgr*)(&src);
			src.that = this;

			src.pub.init_source = init_source;
			src.pub.fill_input_buffer = fill_input_buffer;
			src.pub.skip_input_data = skip_input_data;
			src.pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
			src.pub.term_source = term_source;
			src.pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
			src.pub.next_input_byte = NULL; /* until buffer loaded */
		}


	public:

		enum BufferStatus {
			internalFailedEmpty,   //!< The procedure fails to find delimiter, so the buffer is now empty.
			internalFoundEmpty,    //!< Delimiter has been found, removed, and now the buffer is empty.
			internalFoundNotEmpty  //!< Delimiter has been found, removed, but there is still bytes in the buffer.
		};
		typedef enum BufferStatus BufferStatus;

		Decompress(void) {
			jpeg_copy=(OStream*)NULL;
			input=(IStream*)NULL;
			InitJPEG();
		}

		/**
		* This decompressor will copy the jpeg file in a copy stream. If
		* this stream is a ostringstream, you can have a raw jpeg image
		* stored in memory.
		*/
		Decompress(std::ostream& copy) {
			std_jpeg_copy.setStream(copy);
			jpeg_copy = &std_jpeg_copy;
			InitJPEG();
		}

		/**
		* This decompressor will copy the jpeg file in a copy
		* stream. <b>copy may be deleted internally, it meust have been
		* allocated with new.</b>
		*/
		Decompress(OStream* copy) {
			jpeg_copy=copy;
			InitJPEG();
		}

		~Decompress(void) {
			jpeg_destroy_decompress(&cinfo);
		} 

		/**
		* Set the stream to which the jpeg source will be copied. NULL
		* disables copy.
		*/
		void setCopyStream(OStream* copy) {
			jpeg_copy=copy;
		}

		/**
		* Set the stream to which the jpeg source will be copied. 
		*/
		void setCopyStream(std::ostream& copy) {
			std_jpeg_copy.setStream(copy);
			jpeg_copy = &std_jpeg_copy;
		}

		/**
		* Set the output stream the jpeg files comes from. It has to be
		* correctly opened.
		*/
		void setInputStream(std::istream& is) { 
			std_input.setStream(is);
			input = &std_input;
		}  

		/**
		* Set the output stream the jpeg files comes from. It has to be
		* correctly opened. <b>is may be deleted internally, it must have
		* been allocated with new.</b>
		*/
		void setInputStream(jpeg::IStream* is) { 
			input = is; 
		}  

		/**
		* When decompressing, first call this method, that tells you
		* information about the file, and thus the image buffer size.
		* @param width,height image size. Be sure your destination buffer is big enough.
		* @param depth is 1(gray) or 3(rgb) : this is the number of bytes per pixel.
		*/
		void readHeader(int& width,int& height,
			int& depth, int& dpi) {

				(void) jpeg_read_header(&cinfo,TRUE);
				(void) jpeg_start_decompress(&cinfo);

				depth = cinfo.output_components;
				width = cinfo.output_width;
				height = cinfo.output_height;
				dpi = cinfo.X_density;

				row_stride = width*depth;
			}


			/**
			* Call readHeader once before calling this method.
			* @param img_buf an array of width*height*depth bytes.
			*/
			void readImage(unsigned char* img_buf) {
				unsigned char* row;

				for(row = img_buf;
					cinfo.output_scanline < cinfo.output_height;
					row += row_stride)
					(void) jpeg_read_scanlines(&cinfo,&row,1);
				(void) jpeg_finish_decompress(&cinfo);
			}

			/**
			* Flushes internal buffer. be sure that what is in there isn't
			* the beginning of a next jpeg image.
			*/
			void Flush(void) {
				src.pub.bytes_in_buffer=0;
			}

			/**
			* Eats bytes in the internal buffer until delimiter is found, or
			* internal buffer is empty.
			*/
			BufferStatus FlushUntil(char delimiter) {
				bool not_found;

				for(not_found=true;
					src.pub.bytes_in_buffer>0 && not_found;
					src.pub.next_input_byte++,src.pub.bytes_in_buffer--)
					not_found = (*src.pub.next_input_byte)!=delimiter;

				if(not_found)
					return internalFailedEmpty;
				else if(src.pub.bytes_in_buffer==0)
					return internalFoundEmpty;
				else
					return internalFoundNotEmpty;
			}
	};


}
