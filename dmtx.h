/**
 * libdmtx - Data Matrix Encoding/Decoding Library
 * Copyright 2008, 2009 Mike Laughton. All rights reserved.
 * Copyright 2012-2016 Vadim A. Misbakh-Soloviov. All rights reserved.
 * Copyright 2016 Tim Zaman. All rights reserved.
 *
 * See LICENSE file in the main project directory for full
 * terms of use and distribution.
 *
 * Contact:
 * Vadim A. Misbakh-Soloviov <dmtx@mva.name>
 * Mike Laughton <mike@dragonflylogic.com>
 *
 * \file dmtx.h
 * \brief Main libdmtx header
 */

#ifndef __DMTX_H__
#define __DMTX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Time headers required for DmtxTime struct below */
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifndef M_PI
#define M_PI      3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2    1.57079632679489661923
#endif

#define DmtxVersion              "0.7.5"

#define DmtxUndefined                 -1

// Dmtx 函数返回状态, 通过 Pass 1 或者 Fail 0 
#define DmtxPassFail        unsigned int
#define DmtxPass                       1
#define DmtxFail                       0

// Dmtx bool 值使用 unsigned int 表示
#define DmtxBoolean         unsigned int
#define DmtxTrue                       1
#define DmtxFalse                      0

#define DmtxFormatMatrix               0
#define DmtxFormatMosaic               1

#define DmtxSymbolSquareCount         24
#define DmtxSymbolRectCount            6

#define DmtxModuleOff               0x00
#define DmtxModuleOnRed             0x01
#define DmtxModuleOnGreen           0x02
#define DmtxModuleOnBlue            0x04
#define DmtxModuleOnRGB             0x07  /* OnRed | OnGreen | OnBlue */
#define DmtxModuleOn                0x07
#define DmtxModuleUnsure            0x08
#define DmtxModuleAssigned          0x10
#define DmtxModuleVisited           0x20
#define DmtxModuleData              0x40

#define DMTX_CHECK_BOUNDS(l,i) (assert((i) >= 0 && (i) < (l)->length && (l)->length <= (l)->capacity))

// Dmtx 状态枚举, 正在解码, 完成解码, 解码过程出现了非常规的状态, 解码过程中出现了预期内不可能出现的状态
typedef enum {
   DmtxStatusEncoding, /* Encoding is currently underway */
   DmtxStatusComplete, /* Encoding is done and everything went well */
   DmtxStatusInvalid,  /* Something bad happened that sometimes happens */
   DmtxStatusFatal     /* Something happened that should never happen */
} DmtxStatus;
// Dmtx 的数据编码模式, 字符，字符数字，即不同的模式需要采取不同的方式编码到文件中。
typedef enum {
   DmtxSchemeAutoFast        = -2,
   DmtxSchemeAutoBest        = -1,
   DmtxSchemeAscii           =  0,
   DmtxSchemeC40,
   DmtxSchemeText,
   DmtxSchemeX12,
   DmtxSchemeEdifact,
   DmtxSchemeBase256
} DmtxScheme;
// Dmtx 二维码的单元尺寸, 可以自动, 同时分为了长方形和正方形，单元尺寸(符号尺寸) = 数据单元尺寸 + 识别边界(定位标识 + 时钟标识，可以360度全方位读取)
// 每个码还需要一定量的空白边界
typedef enum {
   DmtxSymbolRectAuto        = -3,
   DmtxSymbolSquareAuto      = -2,
   DmtxSymbolShapeAuto       = -1,
   DmtxSymbol10x10           =  0,
   DmtxSymbol12x12,
   DmtxSymbol14x14,
   DmtxSymbol16x16,
   DmtxSymbol18x18,
   DmtxSymbol20x20,
   DmtxSymbol22x22,
   DmtxSymbol24x24,
   DmtxSymbol26x26,
   DmtxSymbol32x32,
   DmtxSymbol36x36,
   DmtxSymbol40x40,
   DmtxSymbol44x44,
   DmtxSymbol48x48,
   DmtxSymbol52x52,
   DmtxSymbol64x64,
   DmtxSymbol72x72,
   DmtxSymbol80x80,
   DmtxSymbol88x88,
   DmtxSymbol96x96,
   DmtxSymbol104x104,
   DmtxSymbol120x120,
   DmtxSymbol132x132,
   DmtxSymbol144x144,
   DmtxSymbol8x18,
   DmtxSymbol8x32,
   DmtxSymbol12x26,
   DmtxSymbol12x36,
   DmtxSymbol16x36,
   DmtxSymbol16x48
} DmtxSymbolSize;
// Dmtx方向的描述, int 描述, 使用位移操作保留间隔, 可以使用 | 获得组合的方向描述
typedef enum {
   DmtxDirNone               = 0x00,
   DmtxDirUp                 = 0x01 << 0,
   DmtxDirLeft               = 0x01 << 1,
   DmtxDirDown               = 0x01 << 2,
   DmtxDirRight              = 0x01 << 3,
   DmtxDirHorizontal         = DmtxDirLeft  | DmtxDirRight, // 水平 -> 左右
   DmtxDirVertical           = DmtxDirUp    | DmtxDirDown,
   DmtxDirRightUp            = DmtxDirRight | DmtxDirUp, // 东北 -> RightUp
   DmtxDirLeftDown           = DmtxDirLeft  | DmtxDirDown
} DmtxDirection;
// Dmtx码的属性字段枚举
typedef enum {
   DmtxSymAttribSymbolRows,   // 行数
   DmtxSymAttribSymbolCols,   // 列数
   DmtxSymAttribDataRegionRows,  // 数据区的行数
   DmtxSymAttribDataRegionCols,  // 数据区的列数
   DmtxSymAttribHorizDataRegions,
   DmtxSymAttribVertDataRegions,
   DmtxSymAttribMappingMatrixRows,
   DmtxSymAttribMappingMatrixCols,
   DmtxSymAttribInterleavedBlocks,
   DmtxSymAttribBlockErrorWords,
   DmtxSymAttribBlockMaxCorrectable,
   DmtxSymAttribSymbolDataWords,
   DmtxSymAttribSymbolErrorWords,
   DmtxSymAttribSymbolMaxCorrectable
} DmtxSymAttribute;
// Dmtx 角点位置
typedef enum {
   DmtxCorner00              = 0x01 << 0,
   DmtxCorner10              = 0x01 << 1,
   DmtxCorner11              = 0x01 << 2,
   DmtxCorner01              = 0x01 << 3
} DmtxCornerLoc;
// Dmtx算法属性, 编码属性从100开始, 解码属性从200开始, 图像属性从300开始
// 相当于是属性的 getter 函数, 获得属性会使用这个编码来索引
typedef enum {
   /* Encoding properties */
   DmtxPropScheme            = 100,
   DmtxPropSizeRequest,
   DmtxPropMarginSize,
   DmtxPropModuleSize,
   DmtxPropFnc1,
   /* Decoding properties */
   DmtxPropEdgeMin           = 200,
   DmtxPropEdgeMax,
   DmtxPropScanGap,
   DmtxPropSquareDevn,
   DmtxPropSymbolSize,
   DmtxPropEdgeThresh,
   /* Image properties */
   DmtxPropWidth             = 300,
   DmtxPropHeight,
   DmtxPropPixelPacking,
   DmtxPropBitsPerPixel,
   DmtxPropBytesPerPixel,
   DmtxPropRowPadBytes,
   DmtxPropRowSizeBytes,
   DmtxPropImageFlip,
   DmtxPropChannelCount,
   /* Image modifiers */
   DmtxPropXmin              = 400,
   DmtxPropXmax,
   DmtxPropYmin,
   DmtxPropYmax,
   DmtxPropScale
} DmtxProperty;
// Dmtx 打包的顺序, bpp: bit per pixel, 
typedef enum {
   /* Custom format */
   DmtxPackCustom            = 100,
   /* 1 bpp */
   DmtxPack1bppK             = 200,
   /* 8 bpp grayscale */
   DmtxPack8bppK             = 300,
   /* 16 bpp formats */
   DmtxPack16bppRGB          = 400,
   DmtxPack16bppRGBX,
   DmtxPack16bppXRGB,
   DmtxPack16bppBGR,
   DmtxPack16bppBGRX,
   DmtxPack16bppXBGR,
   DmtxPack16bppYCbCr,
   /* 24 bpp formats */
   DmtxPack24bppRGB          = 500,
   DmtxPack24bppBGR,
   DmtxPack24bppYCbCr,
   /* 32 bpp formats */
   DmtxPack32bppRGBX         = 600,
   DmtxPack32bppXRGB,
   DmtxPack32bppBGRX,
   DmtxPack32bppXBGR,
   DmtxPack32bppCMYK
} DmtxPackOrder;
// Dmtx码翻转类型定义
typedef enum {
  DmtxFlipNone               = 0x00,   // 无翻转
  DmtxFlipX                  = 0x01 << 0, // 沿X轴翻转
  DmtxFlipY                  = 0x01 << 1  // 沿Y轴翻转
} DmtxFlip;
// Dmtx 矩阵类型, Matrix3 是三维矩阵, 类型默认使用 double
typedef double DmtxMatrix3[3][3];

// Dmtx 像素坐标类型
typedef struct DmtxPixelLoc_struct {
   int X;
   int Y;
} DmtxPixelLoc;

// Dmtx 二维向量
typedef struct DmtxVector2_struct {
   double          X;
   double          Y;
} DmtxVector2;


// Dmtx 二维射线的表示
typedef struct DmtxRay2_struct {
   double          tMin;
   double          tMax;
   DmtxVector2     p;   // 位置
   DmtxVector2     v;   // 方向
} DmtxRay2;

// Dmtx 一个Byte对应一个 无符号 char
typedef unsigned char DmtxByte;

/**
 * @struct DmtxByteList
 * @brief DmtxByteList
 * 
 */
// Use signed int for length fields instead of size_t to play nicely with RS arithmetic
// 里德-所罗门（Reed-Solomon）算法,用于数据二进制编码 
typedef struct DmtxByteList_struct DmtxByteList;
struct DmtxByteList_struct
{
   int length;
   int capacity;
   DmtxByte *b;
};
// 编码的数据流类型, 描述了当前的编码模式, 下一个未处理的输入单词等
// 同时记录了 Dmtx的状态, 输入输出数据流的二进制表示
typedef struct DmtxEncodeStream_struct DmtxEncodeStream;
struct DmtxEncodeStream_struct
{
   int currentScheme;         /* Current encodation scheme */
   int inputNext;             /* Index of next unprocessed input word in queue */
   int outputChainValueCount; /* Count of output values pushed within current scheme chain */
   int outputChainWordCount;  /* Count of output words pushed within current scheme chain */
   char *reason;              /* Reason for status */
   int sizeIdx;               /* Symbol size of completed stream */
   int fnc1;                  /* Character to represent FNC1, or DmtxUndefined */
   DmtxStatus status;
   DmtxByteList *input;
   DmtxByteList *output;
};

// Dmtx 图像
typedef struct DmtxImage_struct {
   int             width;  // 宽
   int             height; // 高
   int             pixelPacking; // 像素打包方式
   int             bitsPerPixel; // 每个像素用多少 bit 存储
   int             bytesPerPixel;// 每个像素占多少字节
   int             rowPadBytes;  // TODO
   int             rowSizeBytes; // 一行有几个Bytes
   int             imageFlip;    // 图像是否翻转
   int             channelCount; // 通道数
   int             channelStart[4]; // R G B A 的起始位置, 字节为单位
   int             bitsPerChannel[4];  // 每个通道有多少字节, 像素数量*bitsPerPixel
   unsigned char  *pxl;    // 像素的数据区, 用字节表示
} DmtxImage;

// Dmtx 像素点的流动 ? TODO
typedef struct DmtxPointFlow_struct {
   int             plane;
   int             arrive;
   int             depart;
   int             mag;
   DmtxPixelLoc    loc;
} DmtxPointFlow;

// Dmtx直线, BestLine存储在像素平面所拟合的最佳直线
typedef struct DmtxBestLine_struct {
   int             angle;  // TODO
   int             hOffset;
   int             mag;
   int             stepBeg;
   int             stepPos;
   int             stepNeg;
   int             distSq;
   double          devn;
   DmtxPixelLoc    locBeg;
   DmtxPixelLoc    locPos;
   DmtxPixelLoc    locNeg;
} DmtxBestLine;

// Dmtx 码区域的描述
typedef struct DmtxRegion_struct {

   /* Trail blazing values */
   int             jumpToPos;     /* */
   int             jumpToNeg;     /* */
   int             stepsTotal;    /* */
   DmtxPixelLoc    finalPos;      /* */
   DmtxPixelLoc    finalNeg;      /* */
   DmtxPixelLoc    boundMin;      /* */
   DmtxPixelLoc    boundMax;      /* */
   DmtxPointFlow   flowBegin;     /* */

   /* Orientation values */
   int             polarity;      /* */
   int             stepR;
   int             stepT;
   DmtxPixelLoc    locR;          /* remove if stepR works above */
   DmtxPixelLoc    locT;          /* remove if stepT works above */

   /* Region fitting values */
   int             leftKnown;     /* known == 1; unknown == 0 */
   int             leftAngle;     /* hough angle of left edge */
   DmtxPixelLoc    leftLoc;       /* known (arbitrary) location on left edge */
   DmtxBestLine    leftLine;      /* */
   int             bottomKnown;   /* known == 1; unknown == 0 */
   int             bottomAngle;   /* hough angle of bottom edge */
   DmtxPixelLoc    bottomLoc;     /* known (arbitrary) location on bottom edge */
   DmtxBestLine    bottomLine;    /* */
   int             topKnown;      /* known == 1; unknown == 0 */
   int             topAngle;      /* hough angle of top edge */
   DmtxPixelLoc    topLoc;        /* known (arbitrary) location on top edge */
   int             rightKnown;    /* known == 1; unknown == 0 */
   int             rightAngle;    /* hough angle of right edge */
   DmtxPixelLoc    rightLoc;      /* known (arbitrary) location on right edge */

   /* Region calibration values */
   int             onColor;       /* */
   int             offColor;      /* */
   int             sizeIdx;       /* Index of arrays that store Data Matrix constants */
   int             symbolRows;    /* Number of total rows in symbol including alignment patterns */
   int             symbolCols;    /* Number of total columns in symbol including alignment patterns */
   int             mappingRows;   /* Number of data rows in symbol */
   int             mappingCols;   /* Number of data columns in symbol */

   /* Transform values */
   DmtxMatrix3     raw2fit;       /* 3x3 transformation from raw image to fitted barcode grid */
   DmtxMatrix3     fit2raw;       /* 3x3 transformation from fitted barcode grid to raw image */
} DmtxRegion;

// Dmtx 消息的描述, 比如 "01-A103"
typedef struct DmtxMessage_struct {
   size_t          arraySize;     /* mappingRows * mappingCols */
   size_t          codeSize;      /* Size of encoded data (data words + error words) */
   size_t          outputSize;    /* Size of buffer used to hold decoded data */
   int             outputIdx;     /* Internal index used to store output progress */
   int             padCount;
   int             fnc1;          /* Character to represent FNC1, or DmtxUndefined */
   unsigned char  *array;         /* Pointer to internal representation of Data Matrix modules */
   unsigned char  *code;          /* Pointer to internal storage of code words (data and error) */
   unsigned char  *output;        /* Pointer to internal storage of decoded output */
} DmtxMessage;

// Dmtx 扫描网格, Dmtx通过扫描的方式进行Data Matria的识别
typedef struct DmtxScanGrid_struct {
   /* set once */
   int             minExtent;     /* Smallest cross size used in scan */
   int             maxExtent;     /* Size of bounding grid region (2^N - 1) */
   int             xOffset;       /* Offset to obtain image X coordinate */
   int             yOffset;       /* Offset to obtain image Y coordinate */
   int             xMin;          /* Minimum X in image coordinate system */
   int             xMax;          /* Maximum X in image coordinate system */
   int             yMin;          /* Minimum Y in image coordinate system */
   int             yMax;          /* Maximum Y in image coordinate system */

   /* reset for each level */
   int             total;         /* Total number of crosses at this size */
   int             extent;        /* Length/width of cross in pixels */
   int             jumpSize;      /* Distance in pixels between cross centers */
   int             pixelTotal;    /* Total pixel count within an individual cross path */
   int             startPos;      /* X and Y coordinate of first cross center in pattern */

   /* reset for each cross */
   int             pixelCount;    /* Progress (pixel count) within current cross pattern */
   int             xCenter;       /* X center of current cross pattern */
   int             yCenter;       /* Y center of current cross pattern */
} DmtxScanGrid;

// Dmtx 时间的表述
typedef struct DmtxTime_struct {
   time_t          sec;
   unsigned long   usec;
} DmtxTime;

// Dmtx 解码, 需要图像加扫描网格已经一个辅助的 cache 来实现 Data Matrix 码的识别
typedef struct DmtxDecode_struct {
   /* Options */
   int             edgeMin;
   int             edgeMax;
   int             scanGap;
   int             fnc1;
   double          squareDevn;
   int             sizeIdxExpected;
   int             edgeThresh;

   /* Image modifiers */
   int             xMin;
   int             xMax;
   int             yMin;
   int             yMax;
   int             scale;

   /* Internals */
/* int             cacheComplete; */
   unsigned char  *cache;
   DmtxImage      *image;
   DmtxScanGrid    grid;
} DmtxDecode;

// Dmtx 编码
typedef struct DmtxEncode_struct {
   // default : 0
   int             method;
   // default : DmtxSchemeAscii
   int             scheme;
   // default : DmtxSymbolSquareAuto
   int             sizeIdxRequest;
   // default : 10
   int             marginSize;
   // default : 5
   int             moduleSize;
   // default : DmtxPack24bppRGB
   int             pixelPacking;
   // default : DmtxFlipNone
   int             imageFlip;
   // default : 0
   int             rowPadBytes;
   // default : DmtxUndefined
   int             fnc1;
   DmtxMessage    *message;
   DmtxImage      *image;
   DmtxRegion      region;
   DmtxMatrix3     xfrm;  /* XXX still necessary? */
   DmtxMatrix3     rxfrm; /* XXX still necessary? */
} DmtxEncode;

// Dmtx 图像通道
typedef struct DmtxChannel_struct {
   int             encScheme;     /* current encodation scheme */
   int             invalid;       /* channel status (invalid if non-zero) */
   unsigned char  *inputPtr;      /* pointer to current input character */
   unsigned char  *inputStop;     /* pointer to position after final input character */
   int             encodedLength; /* encoded length (units of 2/3 bits) */
   int             currentLength; /* current length (units of 2/3 bits) */
   int             firstCodeWord; /* */
   unsigned char   encodedWords[1558];
} DmtxChannel;

// Wrap in a struct for fast copies, 
typedef struct DmtxChannelGroup_struct {
   DmtxChannel channel[6];
} DmtxChannelGroup;

// 三元组, 一个占一个字节
typedef struct DmtxTriplet_struct {
   unsigned char   value[3];
} DmtxTriplet;

// 四元组, 一个占一个字节 
typedef struct DmtxQuadruplet_struct {
   unsigned char   value[4];
} DmtxQuadruplet;

/* dmtxtime.c */
extern DmtxTime dmtxTimeNow(void);  // 获取当前时间 秒 + 微秒
extern DmtxTime dmtxTimeAdd(DmtxTime t, long msec);  // 向当前时间添加 msec 毫秒
extern int dmtxTimeExceeded(DmtxTime timeout);  // 判断是否超时

/* dmtxencode.c */
extern DmtxEncode *dmtxEncodeCreate(void);   // dmtx 编码对象创建, 相当于构造函数
extern DmtxPassFail dmtxEncodeDestroy(DmtxEncode **enc);   // dmtx 编码对象析构
extern DmtxPassFail dmtxEncodeSetProp(DmtxEncode *enc, int prop, int value);  // dmtx 设置对象属性
extern int dmtxEncodeGetProp(DmtxEncode *enc, int prop); // dmtx 获得对象属性
extern DmtxPassFail dmtxEncodeDataMatrix(DmtxEncode *enc, int n, unsigned char *s); // message ---> data matrix
extern DmtxPassFail dmtxEncodeDataMosaic(DmtxEncode *enc, int n, unsigned char *s); // message ---> data mosaic

/* dmtxdecode.c */
extern DmtxDecode *dmtxDecodeCreate(DmtxImage *img, int scale);   // dmtx 解码对象创建, 相当于构造函数
extern DmtxPassFail dmtxDecodeDestroy(DmtxDecode **dec);   // dmtx 解码对象析构
extern DmtxPassFail dmtxDecodeSetProp(DmtxDecode *dec, int prop, int value);  // dmtx 设置对象属性
extern int dmtxDecodeGetProp(DmtxDecode *dec, int prop); // dmtx 获得对象属性
extern /*@exposed@*/ unsigned char *dmtxDecodeGetCache(DmtxDecode *dec, int x, int y);
extern DmtxPassFail dmtxDecodeGetPixelValue(DmtxDecode *dec, int x, int y, int channel, /*@out@*/ int *value);
extern DmtxMessage *dmtxDecodeMatrixRegion(DmtxDecode *dec, DmtxRegion *reg, int fix); // data matrix ---> message
extern DmtxMessage *dmtxDecodePopulatedArray(int sizeIdx, DmtxMessage *msg, int fix);  // data array ---> message
extern DmtxMessage *dmtxDecodeMosaicRegion(DmtxDecode *dec, DmtxRegion *reg, int fix); // data mosaic ---> message
// 根据解码的信息获得 totalBytes 和 headerBytes, 可以用于校验
extern unsigned char *dmtxDecodeCreateDiagnostic(DmtxDecode *dec, /*@out@*/ int *totalBytes, /*@out@*/ int *headerBytes, int style);

/* dmtxregion.c */
extern DmtxRegion *dmtxRegionCreate(DmtxRegion *reg); // 创建 DmtxRegion
extern DmtxPassFail dmtxRegionDestroy(DmtxRegion **reg); // 析构 DmtxRegion
extern DmtxRegion *dmtxRegionFindNext(DmtxDecode *dec, DmtxTime *timeout); // 检测 DmtxRegion, 只检测单个, 检测到便退出, 同时可以设置最大检测时间
extern DmtxRegion *dmtxRegionScanPixel(DmtxDecode *dec, int x, int y);  // 逐像素扫描, 只检测单个data matrix, 检测到便退出
extern DmtxPassFail dmtxRegionUpdateCorners(DmtxDecode *dec, DmtxRegion *reg, DmtxVector2 p00,
      DmtxVector2 p10, DmtxVector2 p11, DmtxVector2 p01);   // 更新 dmtxRegion 的角点
extern DmtxPassFail dmtxRegionUpdateXfrms(DmtxDecode *dec, DmtxRegion *reg);  // 根据 DmtxRegion 更新检测到的DataMtrix角点像素坐标

/* dmtxmessage.c */
extern DmtxMessage *dmtxMessageCreate(int sizeIdx, int symbolFormat);   // 根据data matrix的格式分配DmtxMessage内存, 格式确定编码的长度都固定了
extern DmtxPassFail dmtxMessageDestroy(DmtxMessage **msg);  // DmtxMessage析构

/* dmtximage.c */
extern DmtxImage *dmtxImageCreate(unsigned char *pxl, int width, int height, int pack);   // 创建图像, 像素数据+编码方式+宽高
extern DmtxPassFail dmtxImageDestroy(DmtxImage **img);   // 图像对象析构
extern DmtxPassFail dmtxImageSetChannel(DmtxImage *img, int channelStart, int bitsPerChannel);  // 一些Image属性的 getter/setter
extern DmtxPassFail dmtxImageSetProp(DmtxImage *img, int prop, int value);
extern int dmtxImageGetProp(DmtxImage *img, int prop);
extern int dmtxImageGetByteOffset(DmtxImage *img, int x, int y);
extern DmtxPassFail dmtxImageGetPixelValue(DmtxImage *img, int x, int y, int channel, /*@out@*/ int *value);
extern DmtxPassFail dmtxImageSetPixelValue(DmtxImage *img, int x, int y, int channel, int value);
extern DmtxBoolean dmtxImageContainsInt(DmtxImage *img, int margin, int x, int y);  // 检测图像坐标是否越界
extern DmtxBoolean dmtxImageContainsFloat(DmtxImage *img, double x, double y);

/* dmtxvector2.c */
extern DmtxVector2 *dmtxVector2AddTo(DmtxVector2 *v1, const DmtxVector2 *v2);
extern DmtxVector2 *dmtxVector2Add(/*@out@*/ DmtxVector2 *vOut, const DmtxVector2 *v1, const DmtxVector2 *v2);
extern DmtxVector2 *dmtxVector2SubFrom(DmtxVector2 *v1, const DmtxVector2 *v2);
extern DmtxVector2 *dmtxVector2Sub(/*@out@*/ DmtxVector2 *vOut, const DmtxVector2 *v1, const DmtxVector2 *v2);
extern DmtxVector2 *dmtxVector2ScaleBy(DmtxVector2 *v, double s);
extern DmtxVector2 *dmtxVector2Scale(/*@out@*/ DmtxVector2 *vOut, const DmtxVector2 *v, double s);
extern double dmtxVector2Cross(const DmtxVector2 *v1, const DmtxVector2 *v2);
extern double dmtxVector2Norm(DmtxVector2 *v);  // 归一化向量
extern double dmtxVector2Dot(const DmtxVector2 *v1, const DmtxVector2 *v2);
extern double dmtxVector2Mag(const DmtxVector2 *v);
extern double dmtxDistanceFromRay2(const DmtxRay2 *r, const DmtxVector2 *q);  // 点q到射线的距离, DmtxRay2的向量默认是归一化的
extern double dmtxDistanceAlongRay2(const DmtxRay2 *r, const DmtxVector2 *q); // 向量 (r->p)q 在射线 r 方向上的投影距离
extern DmtxPassFail dmtxRay2Intersect(/*@out@*/ DmtxVector2 *point, const DmtxRay2 *p0, const DmtxRay2 *p1);  // 求解射线所在直线的交点
extern DmtxPassFail dmtxPointAlongRay2(/*@out@*/ DmtxVector2 *point, const DmtxRay2 *r, double t); // 获得射线 r 方向上, 从起点开始距离加 t 的点坐标point

/* dmtxmatrix3.c */
extern void dmtxMatrix3Copy(/*@out@*/ DmtxMatrix3 m0, DmtxMatrix3 m1);
extern void dmtxMatrix3Identity(/*@out@*/ DmtxMatrix3 m);
extern void dmtxMatrix3Translate(/*@out@*/ DmtxMatrix3 m, double tx, double ty);
extern void dmtxMatrix3Rotate(/*@out@*/ DmtxMatrix3 m, double angle);
extern void dmtxMatrix3Scale(/*@out@*/ DmtxMatrix3 m, double sx, double sy);
extern void dmtxMatrix3Shear(/*@out@*/ DmtxMatrix3 m, double shx, double shy);
extern void dmtxMatrix3LineSkewTop(/*@out@*/ DmtxMatrix3 m, double b0, double b1, double sz);
extern void dmtxMatrix3LineSkewTopInv(/*@out@*/ DmtxMatrix3 m, double b0, double b1, double sz);
extern void dmtxMatrix3LineSkewSide(/*@out@*/ DmtxMatrix3 m, double b0, double b1, double sz);
extern void dmtxMatrix3LineSkewSideInv(/*@out@*/ DmtxMatrix3 m, double b0, double b1, double sz);
extern void dmtxMatrix3Multiply(/*@out@*/ DmtxMatrix3 mOut, DmtxMatrix3 m0, DmtxMatrix3 m1);
extern void dmtxMatrix3MultiplyBy(DmtxMatrix3 m0, DmtxMatrix3 m1);
extern int dmtxMatrix3VMultiply(/*@out@*/ DmtxVector2 *vOut, DmtxVector2 *vIn, DmtxMatrix3 m);
extern int dmtxMatrix3VMultiplyBy(DmtxVector2 *v, DmtxMatrix3 m);
extern void dmtxMatrix3Print(DmtxMatrix3 m);

/* dmtxsymbol.c */
extern int dmtxSymbolModuleStatus(DmtxMessage *mapping, int sizeIdx, int row, int col);
extern int dmtxGetSymbolAttribute(int attribute, int sizeIdx);
extern int dmtxGetBlockDataSize(int sizeIdx, int blockIdx);
extern int getSizeIdxFromSymbolDimension(int rows, int cols);

/* dmtxbytelist.c */
extern DmtxByteList dmtxByteListBuild(DmtxByte *storage, int capacity);
extern void dmtxByteListInit(DmtxByteList *list, int length, DmtxByte value, DmtxPassFail *passFail);
extern void dmtxByteListClear(DmtxByteList *list);
extern DmtxBoolean dmtxByteListHasCapacity(DmtxByteList *list);
extern void dmtxByteListCopy(DmtxByteList *dst, const DmtxByteList *src, DmtxPassFail *passFail);
extern void dmtxByteListPush(DmtxByteList *list, DmtxByte value, DmtxPassFail *passFail);
extern DmtxByte dmtxByteListPop(DmtxByteList *list, DmtxPassFail *passFail);
extern void dmtxByteListPrint(DmtxByteList *list, char *prefix);

extern char *dmtxVersion(void);

#ifdef __cplusplus
}
#endif

#endif
