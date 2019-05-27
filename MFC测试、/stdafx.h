
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include<math.h>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <iterator>
#include <stdlib.h>
#include <algorithm>
#include <io.h>
#include <time.h> 
#include <cmath>
#include<mutex>
#include <string.h>
#include "Eigen/Dense"

#include "KMeans.h"
#include "_RGB2Lab.h"
#include "leftK.h"
#include "leftk2.h"
#include "test.h"
#include "SLIC.h"
#include "util.h"
#include "ICM.h"
#include "GMM.h"
#include "matrix.h"
#define lim0 1.5
#define LIM0 1e-5

#define WM_BASE 10000
#define WM_MESSOK (WM_BASE +1)
#define WM_MESSOK1 (WM_BASE + 2)
#define WM_MESSOK2 (WM_BASE + 3)

#define P2D_ITE map<Point,double>::iterator
#define D2SP_ITE map<double,set<Point>>::iterator

#define INFF 9223372036854775807 
#define INFI 0x3f3f3f3f
#define INFLL  9223372036854775807
#define USERGB
#define SAVEPATH "savelog.log"
#define LABEL5
#ifdef LABEL5
	#define LABEL_NUM 5
	#define LABEL_NUM2 25
#endif
#ifdef LABEL3
	#define LABEL_NUM 3
	#define LABEL_NUM2 9
#endif

#define LINK_N	1
#define N (LINK_N + 1)*(LINK_N + 1)
const double Pi = acos(-1);
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


