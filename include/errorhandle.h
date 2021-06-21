/**
 * \file errorhandle.h
 * \brief �쳣����ͷ�ļ�
 *
 * �����쳣���
 *
 * \author �����
 * \version 1.0
 * \date 2021/06/21
 */

#ifndef ERRORHANDLE_H__
#define ERRORHANDLE_H__

#define SUCCESS 0						///< �ɹ�����	
#define ERROR_XML_PATH 1				///< ·������
#define ERROR_XML_CONFIGURE 2			///< ������Ϣ����
#define ERROR_EMPTY_COLOR 3				///< �޷���ȡɫ����Ϣ
#define ERROR_EMPTY_LINEWIDTH 4			///< �޷���ȡ���������Ϣ	
#define ERROR_PARSING_DATA 5			///< �����ļ�����
#define ERROR_REGISTER_CLASS 6			///< ע�ᴰ�ڴ���

VOID ErrorShow(int error_code);

#endif // !ERRORHANDLE_H__
