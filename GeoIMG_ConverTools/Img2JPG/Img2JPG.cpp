// Img2JPG.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BT_Files.h"
#include "ogrsf_frmts.h"
#include <string.h>



int main(int argc, char **argv)
{
	if (argc < 2)
		return 0;
	path fileFullPath(argv[1]);
	if (!file_exsit(fileFullPath.string()))
	{
		cout << "文件不存在" << endl;
		system("pause");
		return 0;
	}

	string filePath = fileFullPath.string();
	string fileNameWithoutE = fileFullPath.stem().string();
	string savePath = fileFullPath.parent_path().string() + "\\" + fileNameWithoutE + ".jpg";

	if (file_exsit(savePath))
	{
		remove(savePath);
	}

	GDALAllRegister();
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	GDALDataset *poSrcDS = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
	if (poSrcDS == NULL)
	{
		printf("打开图像失败！");
		system("pause");
		return 0;
	}

	//获取图像的基本信息,并把数据读入缓冲区间  
	int m_Width = poSrcDS->GetRasterXSize();
	int m_Height = poSrcDS->GetRasterYSize();
	double* transform = new double[6];
	poSrcDS->GetGeoTransform(transform);

	double s1 = transform[1];	//1
	double s2 = transform[2];	//2
	double s3 = transform[4];	//3
	double s4 = transform[5];	//4
	double s5 = transform[0];	//5
	double s6 = transform[3];	//6
	const char* pro = poSrcDS->GetProjectionRef();

	//把数据保存到临时文件MEM  
	GDALDriver *pDriverMEM = GetGDALDriverManager()->GetDriverByName("MEM");
	GDALDataset *pOutMEMDataset = pDriverMEM->Create("", m_Width, m_Height, 1, GDT_Byte, NULL);
	unsigned char *buffer = new unsigned char[m_Width*m_Height]; //存储数据的缓冲区  
	GDALRasterBand *poSrcDSRasterBand;
	int i = 1;

	while ((poSrcDSRasterBand = poSrcDS->GetRasterBand(i)) != NULL)
	{
		CPLErr err = poSrcDSRasterBand->RasterIO(GF_Read, 0, 0, m_Width, m_Height, buffer, m_Width, m_Height, GDT_Byte, 0, 0);
		if (err != CE_None)
		{
			printf("读取图像数据失败！");
			return 0;
		}
		/* 以下程序是生成JPG的过程，上面的程序只为获得创建过程中的一些参数，如：图像的高、宽及图像数据*/

		GDALRasterBand *pOutMEMRasterBand = pOutMEMDataset->GetRasterBand(i);
		if (pOutMEMRasterBand == NULL)
		{
			pOutMEMDataset->AddBand(GDT_Byte);
			pOutMEMRasterBand = pOutMEMDataset->GetRasterBand(i);
		}
		err = pOutMEMRasterBand->RasterIO(GF_Write, 0, 0, m_Width, m_Height, buffer, m_Width, m_Height, GDT_Byte, 0, 0);
		if (err != CE_None)
		{
			printf("写图像数据失败！");
			return 0;
		}
		i++;
	}
	//以创建复制的方式，生成jpg文件  
	GDALDriver *pDriverJPG = GetGDALDriverManager()->GetDriverByName("JPEG");
	pDriverJPG->CreateCopy(savePath.c_str(), pOutMEMDataset, TRUE, 0, 0, 0); //创建jpg文件  

	//system("pause");
	return 0;
}

