#define _CRT_SECURE_NO_WARNINGS
//#undef __cplusplus
extern "C"{
#include "cserial.h"
#include<windows.h>
#include<windowsx.h>
#include<stdio.h>
#include"resource.h"
#include"S_resource.h"
#include"Definitions.h"
#include<commctrl.h>
#include <time.h>
#include <stdlib.h>

#include "LeapC.h"
#include "ExampleConnection.h"
#define PI 3.14159265359

#pragma comment(lib, "winmm.lib")//timeGetTime()�g���̂ɕK�v
#pragma comment ( lib , "COMCTL32.LIB" )//Epos�g���̂ɕK�v
#pragma comment ( lib , "EposCmd.lib" )//Epos�g���̂ɕK�v
#pragma comment ( lib , "EposCmd64.lib" )//Epos�g���̂ɕK�v

CSerial* cserial; 								// CSerial�N���X�̃|�C���^�̐錾

//�_�C�A���O�֐��̃v���g�^�C�v�錾
BOOL CALLBACK DialogFunc(HWND, UINT, WPARAM, LPARAM);//�ڍאݒ�
BOOL CALLBACK DialogFunc1(HWND, UINT, WPARAM, LPARAM);//�ڍאݒ�̃��n�r�����
BOOL CALLBACK DialogSTART(HWND, UINT, WPARAM, LPARAM);//�������
BOOL CALLBACK DialogSIMPLEREHA(HWND, UINT, WPARAM, LPARAM);//���n�r�����
BOOL CALLBACK DialogLeap(HWND, UINT, WPARAM, LPARAM);//Leap�ݒ�

BOOL CALLBACK DialogS(HWND, UINT, WPARAM, LPARAM);//�Z���T�[�����_�C�A���O
BOOL CALLBACK DialogSS(HWND, UINT, WPARAM, LPARAM);//�V�^�Z���T�[
BOOL CALLBACK DialogKS(HWND, UINT, WPARAM, LPARAM);//���^�Z���T�[
BOOL CALLBACK DialogRS(HWND, UINT, WPARAM, LPARAM);//���n�r����������I��


int grab = 0;
int c = 0;
#define sec 4 //���삳�������b(�Г�)
char* DeviceName = "EPOS2";
char* protocolstackName = "MAXON SERIAL V2";
char* interfaceName = "USB";
char* portName = "USB0";
/*char* DeviceName = "EPOS2";
char* protocolstackName = "CANopen";
char* interfaceName = "Vector_VN1610 Channel 1";
char* portName = "CAN0";*/
DWORD errorCode = 0;
WORD NodeId = 1;
long ActualValue[10];//100ms���ƂɎ擾���錻�݂̒l
long TestValue[10];
int Connection_Check[10] = { 0 };//�ڑ��������Ƀt���O�𗧂��グ��
unsigned int Slider_Value = 0;//�z�[����ʂ̃X���C�h�R���g���[���̒l
unsigned int R_Slider_Value = 2048;//���n�r���̃X���C�h�R���g���[���̒l
int R_Target = 50;//���n�r���̖ڕW��
unsigned int R_Num = 0;//���n�r����
unsigned int i = 0, j = 0, k = 0, h = 10, r = 10, rh = 10, e = 10, rf = 10, re = 10;//���[�v�Ɏg���ϐ�
int test_flag = 0;
long Home[10] = { 1 };//Homing mode�̃t���O
int Save_value[10];//�ۑ�����l
int Read_value[10];//�ǂݏo���l
int Calcu_value[10] = { 0 };//Flexion_value��Extension_value�̍���ۑ����Čv�Z�Ɏg��
int Test_Calcu[3][10] = { 0 };
int Flexion_value[10] = { 0 };//���ȂɎg�p����ǂݏo�����l
int Extension_value[10] = { 0 };//�L�W�Ɏg�p����ǂݏo�����l

int Flexion_Value[9][10] = { 0 };//���ȂS��ނ̓ǂݏo�����l
int Extension_Value[9][10] = { 0 };//�L�W�S��ނ̓ǂݏo�����l
int Rotation_Value[6][10] = { 0 };
int counter_flag = 0;
int hara = 0;
int Rotation_value[4][10] = { 0 };//��]�Ɏg�p����ǂݏo�����l  [a�`d][10�@]
int Now_position = 0;//a�`d�̔��f�p�ϐ� ���݈ʒu
int Next_position = 1;//�ڕW�ʒu

int Now_rot_position = 0;
int Next_rot_position = 1;
int New_Threshold = 0;//�L���Z���T(�O���[)�̒l�������肫��Ȃ��̂ŁA�������I������ۂ̒l+50��V����臒l�ɂ��邽�߂̕ϐ�

char Save_Buffer[70];//�l��char�ɕϊ����Ĉꎞ�ۑ�����
char Read_Buffer[70];//�l��ǂݏo���Ĉꎞ�ۑ�����
char tmp[7];//�ǂݏo�����l��int�ɒ������߈ꎞ�i�[����
bool Read_flag = 0;//�t�@�C����ǂݏo���������肷��

int Rot_flag = 0;//��w��]�f�[�^a�`d����p�@
int Read_flag_R = 0;//��]�p

int rep[6] = { 0 };//�����p
int rep_det[6] = { 0 };
int rep_reset = 1;
int REPRIHA = 0;
int Rep_flag = 0;

WORD Tension_value[10] = { 0 };//�A�i���O�l���i�[����ϐ�
WORD Before_Tension_value[10] = { 0 };//�A�i���O�l���i�[����ϐ�
WORD Before_Tension_value2[10] = { 0 };//�A�i���O�l���i�[����ϐ�
int Safety_value[10] = { 1500, 3000, 1300, 1300, 1600, 2500, 1600, 1700, 3500, 5000 };//�����̓Z���T��臒l
int Safety_mode[10] = { 0 };//�����̓Z���T��臒l�ȏ�ɂȂ��Ă��邩���肷��
int Max_value[10] = { -48640, -48640, -48640, -48640, -48640, -48640, -48640, -48640, -48640, -48640 };//�G���R�[�_�̍ő�l�@190mm�ɐݒ�
int Reach_check[12] = { 0 };//0~9�͖ڕW�l�ɓ��B�������m�F�A10�͐ڑ����u���A11�͖ڕW�l�ɓ��B�������u�̐��̃J�E���g
int Reha_end = 1;//�����I���̔��� 1�ŏI��
int Reha_close = 0;//���n�r����ʂő��̓��삪�I����Ă邩���肷��ϐ��@0:����� 1:���쒆
long Get_value[10] = { 0 };//�w���쒆�̒l
int Sensor_data[14] = { 0 };//�؎��k�̃f�[�^0~1�A�ؓd�p�̃f�[�^:2~5�A�Ȃ��Z���T�̃f�[�^6~12�AMSP�̃f�[�^13
int Sensor_Reha_Flag = 0;//�Z���T�[��臒l�𒴂��Ă��邩�̔��� 0:臒l�ȉ� 1:臒l�ȏ�
int Before_Sensor_Reha_Flag = 0;//�Z���T�[�̃`���^�����O�h���I��
int Halt_check[10] = { 0 };//���f�������m�F
WORD AD_TMP[10] = { 0 };//�����ݒ�̎��Ɉꎞ�I��AD�l��ۑ�����ϐ�
int Auto_setting = 0;//�I�[�g�ݒ�̍ۂ̕ϐ��@1,�L�W�܂��͋��Ȃ̎������� 2,1�Ɣ��΂̎������� 0,�ݒ芮���@
int Auto_check[10] = { 0 };//�I�[�g�ݒ肪�I�����������
int Used_Sensor[12] = { 0 };//�g�p����Z���T�[�̔��� 0:�؎��k 1~4:�ؓd�@5~11:�Ȃ�
int Used_Sensor_amount = 0;//���n�r���̍ۂɎg�p����Z���T�[�̐�
int Exceed_Sensor_amount = 0;//臒l�𒴂����C���^�[�t�F�[�X�Z���T�[�̍��v���𐔂���
int fileopen = 0;//�t�@�C���I�[�v�����Ă��邩�̔���@0:���Ă��Ȃ��@1:���Ă���
int Dlg_Sensor[12] = { 0 };//�_�C�A���O����擾�����C���^�[�t�F�[�X�Z���T�[�̒l
int Dlg_Th[12] = { 1,1,1,1,1,1,1,1,1,1,1,1 };//�_�C�A���O����擾����臒l
int Exceed_falg[12] = { 0 };//�C���^�[�t�F�[�X�Z���T�[�̃`���^�����O�h���I��
long Reha_Motor[10] = { 0 };//���n�r�����̃G���R�[�_�̒l
int Ext_Sensor_mode = 0;//�Z���T�[�L�W���n�r�����̃C���^�[�t�F�[�X�Z���T�[�𔽉f�����邩�̔���
int Fle_Sensor_mode = 0;//�Z���T�[���ȃ��n�r�����̃C���^�[�t�F�[�X�Z���T�[�𔽉f�����邩�̔���
int Reha_step = 0;//���n�r���̒i�K�̔���@0:�����ʒu�܂őҋ@�@1:����or�L�W�܂őҋ@ �@2:�L�Wor���Ȃ܂őҋ@
int Rot_step = 0;//���n�r����w��]�̒i�K����
int a = 0;
int b = 0;


FILE* fp = NULL;
time_t now;
char File[256];
struct tm* date;

DWORD S;//���߂̎���
DWORD E;//�I���̎��� E-S�Ōo�ߎ���

HWND start;
MSG msg;

HWND hwndSR;
HINSTANCE simplereha;

HWND hwnd;
HINSTANCE home;

HWND hwndR;
HINSTANCE rehabili;


HWND hwndRS;
HINSTANCE reptition;

HWND hwndS;
HINSTANCE sensor;

HWND hwndSS;
HINSTANCE singata;

HWND hwndKS;
HINSTANCE kyugata;

HWND hwndL;
HINSTANCE leap;


HANDLE keyHandle = VCS_OpenDevice(DeviceName, protocolstackName, interfaceName, portName, &errorCode);


DWORD WINAPI Normal_Reha_Thread(LPVOID vdParam) {//���񏈗��Ń��[�^�[�𓮂����ă��n�r������(�ڍאݒ�̕�)
	while (Reach_check[10] != Reach_check[11]) {
		for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
			if (Connection_Check[i] == 1) {
				if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
					Reach_check[i] = 1;
					Reach_check[11]++;
				}
				VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);//�����̓Z���T�̌��݂̒l
				SetDlgItemInt(hwndR, Reha_sensor1 + i, Tension_value[i], TRUE);
				if (Safety_value[i] <= Tension_value[i]) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
					VCS_MoveToPosition(keyHandle, NodeId + i, Get_value[i], 1, 0, &errorCode);
					Extension_value[i] = Get_value[i];
					SetDlgItemInt(hwndR, Ext_value1 + i, Extension_value[i] / -256, TRUE);
				}
				if (Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
					for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
						if (Connection_Check[i] == 1) {
							VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
							VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
							VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						}
					}
					EnableWindow(GetDlgItem(hwndR, IDC_FlexionData), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_ExtensionData), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
					Reach_check[11] = 0;
					Reha_close = 0;
					R_Num = 0;
					SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
					ExitThread(TRUE);
				}
			}
		}
	}
	if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//�e�w�����

		//�ڕW�l�܂Ŏw�肵�����Ԃœ�����
		for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
			if (Connection_Check[i] == 1) {
				Calcu_value[i] = ((-15 * Extension_value[i]) / (sec * sec * 32)) - ((-15 * Flexion_value[i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
				if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
				VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
			}
		}
	}
	while (TRUE) {
		//���ȓ���
		if (Reha_end == 0) {
			if (Reach_check[10] == Reach_check[11]) {
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
					if (Connection_Check[i] == 1) { //�ڑ������Ƃ��Ƀt���O�𗧂��グ��
						Reach_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_value[i], 1, 0, &errorCode);
					}
				}
			}
			while (Reach_check[10] != Reach_check[11]) {
				for (i = 0; i < 10; i++) {//�S�ċ��Ȃ���܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
						VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);////�����̓Z���T�̌��݂̒l
						SetDlgItemInt(hwndR, Reha_sensor1 + i, Tension_value[i], TRUE);//
						if (Safety_value[i] <= Tension_value[i]) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
							VCS_MoveToPosition(keyHandle, NodeId + i, Get_value[i], 1, 0, &errorCode);
							Flexion_value[i] = Get_value[i];
							SetDlgItemInt(hwndR, Fle_value1 + i, Flexion_value[i] / -256, TRUE);
						}
						if (Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
							for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
								if (Connection_Check[i] == 1) {
									VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
									VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
									VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
								}
							}
							EnableWindow(GetDlgItem(hwndR, IDC_FlexionData), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_ExtensionData), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
							Reach_check[11] = 0;
							Reha_close = 0;
							R_Num = 0;
							SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
							ExitThread(TRUE);
						}
					}
				}
			}
		}
		//�L�W����
		if (Reha_end == 0) {
			if (Reach_check[10] == Reach_check[11]) {
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
					if (Connection_Check[i] == 1) {
						Reach_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Extension_value[i], 1, 0, &errorCode);
					}
				}
			}
			while (Reach_check[10] != Reach_check[11]) {
				for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
						VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);//�����̓Z���T�̌��݂̒l
						SetDlgItemInt(hwndR, Reha_sensor1 + i, Tension_value[i], TRUE);
						if (Safety_value[i] <= Tension_value[i]) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
							VCS_MoveToPosition(keyHandle, NodeId + i, Get_value[i], 1, 0, &errorCode);
							Extension_value[i] = Get_value[i];
							SetDlgItemInt(hwndR, Ext_value1 + i, Extension_value[i] / -256, TRUE);
						}
						if (Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
							for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
								if (Connection_Check[i] == 1) {
									VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
									VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
									VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
								}
							}
							EnableWindow(GetDlgItem(hwndR, IDC_FlexionData), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_ExtensionData), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
							Reach_check[11] = 0;
							Reha_close = 0;
							R_Num = 0;
							SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
							ExitThread(TRUE);
						}
					}
				}
			}
		}
		//����1�����I�����Ƃ̏���
		if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
			R_Num++;
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
		}
		if (R_Target <= R_Num || Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
			//�I�����̏���
			for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
				}
			}
			EnableWindow(GetDlgItem(hwndR, IDC_FlexionData), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_ExtensionData), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
			Reach_check[11] = 0;
			Reha_end = 0;
			Reha_close = 0;
			R_Num = 0;
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
			ExitThread(TRUE);
		}
	}
}

//�d�w��]
DWORD WINAPI Rotation_Reha_Thread(LPVOID vdParam) {//���񏈗��Ń��[�^�[�𓮂����ă��n�r������(�ڍאݒ�̕�)
	while (Reach_check[10] != Reach_check[11]) {
		for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
			if (Connection_Check[i] == 1) {
				if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
					Reach_check[i] = 1;
					Reach_check[11]++;
				}
				VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);//�����̓Z���T�̌��݂̒l
				SetDlgItemInt(hwndR, Reha_sensor1 + i, Tension_value[i], TRUE);
				/*
								if (Safety_value[i] <= Tension_value[i]) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
									VCS_MoveToPosition(keyHandle, NodeId + i, Get_value[i], 1, 0, &errorCode);
									Rotation_value[Now_position][i] = Get_value[i];
									SetDlgItemInt(hwndR, Rotation_value1 + i + (Now_position * 10), Rotation_value[Now_position][i] / -256, TRUE);
								}
				*/
				if (Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
					for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
						if (Connection_Check[i] == 1) {
							VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
							VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
							VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						}
					}
					EnableWindow(GetDlgItem(hwndR, IDC_Rotation_a), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_Rotation_b), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_Rotation_c), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_Rotation_d), 1);
					EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
					Reach_check[11] = 0;
					Reha_close = 0;
					R_Num = 0;
					SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
					ExitThread(TRUE);
				}
			}
		}
	}
	if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//��]�����
		for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
			if (Connection_Check[i] == 1) {
				Calcu_value[i] = ((-15 * Rotation_value[Now_position][i]) / (sec * sec * 32)) - ((-15 * Rotation_value[Next_position][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
				if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
				VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
			}
		}
	}
	while (TRUE) {
		//���� Now��Next
		if (Reha_end == 0) {
			if (Reach_check[10] == Reach_check[11]) {
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
					if (Connection_Check[i] == 1) {
						Reach_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_value[Next_position][i], 1, 0, &errorCode);
					}
				}
			}
			while (Reach_check[10] != Reach_check[11]) {
				for (i = 0; i < 10; i++) {//�S�Ĉړ�����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
						VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);////�����̓Z���T�̌��݂̒l
						SetDlgItemInt(hwndR, Reha_sensor1 + i, Tension_value[i], TRUE);//

	/*
						if (Safety_value[i] <= Tension_value[i]) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
							VCS_MoveToPosition(keyHandle, NodeId + i, Get_value[i], 1, 0, &errorCode);
							Rotation_value[Next_position][i] = Get_value[i];
							SetDlgItemInt(hwndR, Rotation_value1 + i + (Next_position * 10), Rotation_value[Next_position][i] / -256, TRUE);
						}
	*/

						if (Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
							for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
								if (Connection_Check[i] == 1) {
									VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
									VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
									VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
								}
							}


							EnableWindow(GetDlgItem(hwndR, IDC_Rotation_a), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_Rotation_b), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_Rotation_c), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_Rotation_d), 1);
							EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
							Reach_check[11] = 0;
							Reha_close = 0;
							R_Num = 0;
							SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
							ExitThread(TRUE);
						}
					}
				}
			}
		}



		//1����I�����Ƃ̏���
		if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
			//1��]�I����
			if (Now_position == 3) {
				R_Num++;
				SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
			}

			Now_position++;
			Now_position %= 4;

			Next_position++;
			Next_position %= 4;

			if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//���̎p����
				for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
					if (Connection_Check[i] == 1) {
						Calcu_value[i] = ((-15 * Rotation_value[Now_position][i]) / (sec * sec * 32)) - ((-15 * Rotation_value[Next_position][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
						if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
						VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
					}
				}
			}
		}

		if (R_Target <= R_Num || Reha_end == 1 && Reach_check[10] == Reach_check[11]) {//���n�r���I���̏���
																					   //�I�����̏���
			for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 0, &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
				}
			}
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_a), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_b), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_c), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_d), 1);
			EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
			Reach_check[11] = 0;
			Reha_end = 1;
			Reha_close = 0;
			R_Num = 0;
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, R_Num, TRUE);
			ExitThread(TRUE);
		}
	}
}


static VOID funcFileSave(HWND hWnd)//save
{
	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];
	HANDLE fw;

	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
		ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".csv");
		ofn.lpstrFilter = TEXT("csv�t�@�C��(*.csv)\0*.csv\0");
		ofn.lpstrTitle = TEXT("csv�t�@�C����ۑ����܂��B");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		fw = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(fw, Save_Buffer, lstrlen(Save_Buffer), NULL, NULL);
		CloseHandle(fw);

		MessageBox(hWnd, szFile, TEXT("�t�@�C������t���ĕۑ�"), MB_OK);
	}
}

static VOID funcFileOpen(HWND hWnd)//open
{
	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];
	HANDLE fr;

	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
		ofn.lpstrFile = szFile;       // �I���t�@�C���i�[/
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = TEXT("csv�t�@�C��(*.csv)\0*.csv\0");
		ofn.lpstrTitle = TEXT("csv�t�@�C����I�����܂��B");
		ofn.Flags = OFN_FILEMUSTEXIST;
	}
	if (GetOpenFileName(&ofn)) {
		Read_flag = 1;
		for (i = 0; i < 70; i++) { Read_Buffer[i] = 0; }
		fr = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(fr, Read_Buffer, GetFileSize(fr, NULL), NULL, NULL);
		CloseHandle(fr);
		MessageBox(hWnd, szFile, TEXT("�t�@�C�����J��"), MB_OK);
	}
	else {
		for (i = 0; i <= 20; i++) {
			if (i == 20) {
				Read_Buffer[i] = NULL;
				break;
			}
			if (i % 2 == 0) {
				Read_Buffer[i] = '0';
			}
			else {
				Read_Buffer[i] = ',';
			}
		}
	}
}


int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	//(1)���\�[�X�Ɋ�Â��_�C�A���O�{�b�N�X���쐬����B�����ō�����_�C�A���O�� ID �����킹��悤�ɁB
	start = CreateDialog(hThisInst, "START", NULL, (DLGPROC)DialogSTART);
	//�_�C�A���O�{�b�N�X���\������Ȃ��ꍇ�̓v���O�������I������B
	if (start == NULL)
	{
		MessageBox(NULL, "�_�C�A���O�{�b�N�X�� ID ���Ԉ���Ă��܂���?", "�I��", MB_OK);
		return 0;
	}
	//(2)�_�C�A���O�{�b�N�X��\������B 
	ShowWindow(start, SW_SHOW);
	UpdateWindow(start);
	//(3)���b�Z�[�W���[�v�̎��s���J�n����(�_�C�A���O�֐��̏������s��)�B 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	// �L�[�{�[�h���b�Z�[�W��ϊ����� 
		DispatchMessage(&msg);  // Windows 2000 �ɐ����߂� 
	}
	return msg.wParam;
}

//HOME�@�ڍאݒ�
//�_�C�A���O�֐� 
BOOL CALLBACK DialogFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:             // WM_COMMAND �̓R���g���[��(�{�^���Ȃ�)���I�����ꂽ�ꍇ�ɔ�������B
		switch (LOWORD(wParam))  // LOWORD( wParam )�ɂ̓R���g���[��(�{�^���Ȃ�)�� ID ���������Ă���B
		{
		case IDC_End:
			KillTimer(hwnd, 1);
			SetTimer(start, 1, 100, NULL);
			DestroyWindow(hwnd);
			return	1;

		case IDC_ALL_Home:IDC_EDIT1;
			for (i = 0; i <= 9; i++) {
				VCS_ClearFault(keyHandle, NodeId + i, &errorCode);
				if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
				{
					if (VCS_SetHomingParameter(keyHandle, NodeId + i, 10000, 1000, 1, 0, 500, 0, &errorCode))
					{
						if (VCS_SetEnableState(keyHandle, NodeId + i, &errorCode))
						{
							VCS_FindHome(keyHandle, NodeId + i, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
							SetTimer(hwnd, 2 + i, 100, NULL);
						}
					}
				}
			}
			return 1;

		case IDC_ALL_Connection:
			for (i = 0; i <= 9; i++) {
				if (IsWindowEnabled(GetDlgItem(hwnd, IDC_Connection1 + i))) {//�{�^���̏�Ԃ��P�Ȃ�ɂ�����
					Connection_Check[i] = 1;
					EnableWindow(GetDlgItem(hwnd, IDC_Disconnection1 + i), 1);
					EnableWindow(GetDlgItem(hwnd, IDC_Connection1 + i), 0);
					EnableWindow(GetDlgItem(hwnd, IDC_RESET1 + i), 1);
					if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_PROFILE_POSITION_MODE, &errorCode))
					{
						if (VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode))
						{
							if (VCS_SetEnableState(keyHandle, NodeId + i, &errorCode))
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
								SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
							}
						}
					}
				}
			}
			return 1;

		case IDC_ALL_Disconnection:
			for (i = 0; i <= 9; i++) {
				if (IsWindowEnabled(GetDlgItem(hwnd, IDC_Disconnection1 + i))) {//�{�^���̏�Ԃ��P�Ȃ�
					Connection_Check[i] = 0;
					EnableWindow(GetDlgItem(hwnd, IDC_Disconnection1 + i), 0);
					EnableWindow(GetDlgItem(hwnd, IDC_Connection1 + i), 1);
					EnableWindow(GetDlgItem(hwnd, IDC_RESET1 + i), 0);
					VCS_SetDisableState(keyHandle, NodeId + i, &errorCode);
				}
			}
			return 1;

		case IDC_Home1:
			VCS_ClearFault(keyHandle, NodeId, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 2, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home2:
			VCS_ClearFault(keyHandle, NodeId + 1, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 1, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 1, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 1, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 1, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 3, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home3:
			VCS_ClearFault(keyHandle, NodeId + 2, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 2, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 2, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 2, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 2, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 4, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home4:
			VCS_ClearFault(keyHandle, NodeId + 3, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 3, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 3, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 3, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 3, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 5, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home5:
			VCS_ClearFault(keyHandle, NodeId + 4, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 4, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 4, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 4, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 4, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 6, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home6:
			VCS_ClearFault(keyHandle, NodeId + 5, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 5, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 5, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 5, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 5, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 7, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home7:
			VCS_ClearFault(keyHandle, NodeId + 6, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 6, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 6, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 6, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 6, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 8, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home8:
			VCS_ClearFault(keyHandle, NodeId + 7, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 7, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 7, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 7, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 7, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 9, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home9:
			VCS_ClearFault(keyHandle, NodeId + 8, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 8, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 8, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 8, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 8, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 10, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Home10:
			VCS_ClearFault(keyHandle, NodeId + 9, &errorCode);
			if (VCS_SetOperationMode(keyHandle, NodeId + 9, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
			{
				if (VCS_SetHomingParameter(keyHandle, NodeId + 9, 10000, 1000, 1, 0, 500, 0, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 9, &errorCode))
					{
						VCS_FindHome(keyHandle, NodeId + 9, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
						SetTimer(hwnd, 11, 100, NULL);
					}
				}
			}
			return 1;

		case IDC_Connection1:
			Connection_Check[0] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection1), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection1), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET1), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection2:
			Connection_Check[1] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection2), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection2), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET2), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 1, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 1, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 1, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 1, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER2), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection3:
			Connection_Check[2] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection3), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection3), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET3), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 2, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 2, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 2, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER3), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection4:
			Connection_Check[3] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection4), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection4), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET4), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 3, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 3, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 3, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER4), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection5:
			Connection_Check[4] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection5), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection5), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET5), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 4, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 4, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 4, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 4, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER5), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection6:
			Connection_Check[5] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection6), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection6), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET6), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 5, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 5, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 5, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 5, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER6), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection7:
			Connection_Check[6] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection7), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection7), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET7), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 6, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 6, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 6, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 6, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER7), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection8:
			Connection_Check[7] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection8), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection8), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET8), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 7, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 7, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 7, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 7, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER8), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection9:
			Connection_Check[8] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection9), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection9), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET9), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 8, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 8, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 8, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 8, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER9), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Connection10:
			Connection_Check[9] = 1;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection10), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection10), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET10), 1);
			if (VCS_SetOperationMode(keyHandle, NodeId + 9, OMD_PROFILE_POSITION_MODE, &errorCode))
			{
				if (VCS_SetPositionProfile(keyHandle, NodeId + 9, 2400, 3200, 3200, &errorCode))
				{
					if (VCS_SetEnableState(keyHandle, NodeId + 9, &errorCode))
					{
						VCS_MoveToPosition(keyHandle, NodeId + 9, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER10), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}
					return 1;
				}
			}
			VCS_CloseDevice(keyHandle, &errorCode);
			return 1;

		case IDC_Disconnection1:
			Connection_Check[0] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection1), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection1), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET1), 0);
			VCS_SetDisableState(keyHandle, NodeId, &errorCode);
			return 1;

		case IDC_Disconnection2:
			Connection_Check[1] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection2), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection2), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET2), 0);
			VCS_SetDisableState(keyHandle, NodeId + 1, &errorCode);
			return 1;

		case IDC_Disconnection3:
			Connection_Check[2] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection3), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection3), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET3), 0);
			VCS_SetDisableState(keyHandle, NodeId + 2, &errorCode);
			return 1;

		case IDC_Disconnection4:
			Connection_Check[3] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection4), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection4), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET4), 0);
			VCS_SetDisableState(keyHandle, NodeId + 3, &errorCode);
			return 1;

		case IDC_Disconnection5:
			Connection_Check[4] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection5), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection5), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET5), 0);
			VCS_SetDisableState(keyHandle, NodeId + 4, &errorCode);
			return 1;

		case IDC_Disconnection6:
			Connection_Check[5] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection6), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection6), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET6), 0);
			VCS_SetDisableState(keyHandle, NodeId + 5, &errorCode);
			return 1;

		case IDC_Disconnection7:
			Connection_Check[6] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection7), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection7), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET7), 0);
			VCS_SetDisableState(keyHandle, NodeId + 6, &errorCode);
			return 1;

		case IDC_Disconnection8:
			Connection_Check[7] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection8), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection8), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET8), 0);
			VCS_SetDisableState(keyHandle, NodeId + 7, &errorCode);
			return 1;

		case IDC_Disconnection9:
			Connection_Check[8] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection9), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection9), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET9), 0);
			VCS_SetDisableState(keyHandle, NodeId + 8, &errorCode);
			return 1;

		case IDC_Disconnection10:
			Connection_Check[9] = 0;
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection10), 0);
			EnableWindow(GetDlgItem(hwnd, IDC_Connection10), 1);
			EnableWindow(GetDlgItem(hwnd, IDC_RESET10), 0);
			VCS_SetDisableState(keyHandle, NodeId + 9, &errorCode);
			return 1;

		case IDC_ALLRESET:
			for (i = 0; i < 10; i++) {
				VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
				SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			}
			return 1;

		case IDC_RESET1:
			VCS_MoveToPosition(keyHandle, NodeId, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET2:
			VCS_MoveToPosition(keyHandle, NodeId + 1, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 1), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET3:
			VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 2), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET4:
			VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 3), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET5:
			VCS_MoveToPosition(keyHandle, NodeId + 4, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 4), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET6:
			VCS_MoveToPosition(keyHandle, NodeId + 5, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 5), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET7:
			VCS_MoveToPosition(keyHandle, NodeId + 6, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 6), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET8:
			VCS_MoveToPosition(keyHandle, NodeId + 7, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 7), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET9:
			VCS_MoveToPosition(keyHandle, NodeId + 8, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 8), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_RESET10:
			VCS_MoveToPosition(keyHandle, NodeId + 9, 0, 1, 1, &errorCode);
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + 9), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			return 1;

		case IDC_Rehabili:
			Reach_check[10] = 0;
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					Reach_check[10]++;
					VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
				}
			}
			hwndR = CreateDialog(rehabili, "REHABILI", NULL, (DLGPROC)DialogFunc1);
			if (hwndR == NULL)
			{
				MessageBox(NULL, "���s�ł��܂���", "�I��", MB_OK);
				return 0;
			}
			KillTimer(hwnd, 1);
			ShowWindow(hwndR, SW_SHOW);
			UpdateWindow(hwndR);
			return 1;

		case IDC_READ:
			funcFileOpen(hwnd);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Read_value[k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Read_value[k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_MoveToPosition(keyHandle, NodeId + i, Read_value[i], 1, 1, &errorCode);
					SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, Read_value[i] / -256);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
				}
			}
			return 1;

		case IDC_Save:
			for (i = 0; i < 10; i++) {
				Save_value[i] = GetDlgItemInt(hwnd, IDC_EDIT1 + i, NULL, TRUE) * -256;
			}
			sprintf_s(Save_Buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", Save_value[0], Save_value[1], Save_value[2], Save_value[3], Save_value[4], Save_value[5], Save_value[6], Save_value[7], Save_value[8], Save_value[9]);
			funcFileSave(hwnd);
			return 1;

		case IDC_BUTTON1:
			a = GetDlgItemInt(hwnd, IDC_EDIT41, NULL, TRUE);
			VCS_MoveToPosition(keyHandle, NodeId, a * -256, 1, 1, &errorCode);
			SetDlgItemInt(hwnd, IDC_EDIT42, a, TRUE);
			return 1;
		}

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 1://���݂̒l���_�C�A���O�ɕ\��
			for (i = 0; i < 10; i++) {

				if (Connection_Check[i] == 1) {
					VCS_GetPositionIs(keyHandle, NodeId + i, &ActualValue[i], &errorCode);//���݂̃G���R�[�_�̒l���擾
					SetDlgItemInt(hwnd, IDC_EDIT1 + i, ActualValue[i] / -256, TRUE);
					if (Max_value[i] / 256 > ActualValue[i] / 256) {//�ő�l���G���R�[�_���������烂�[�^�[��0�� 
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
					}

					VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);//�����̓Z���T�̌��݂̒l
					SetDlgItemInt(hwnd, IDC_EDIT21 + i, Tension_value[i], TRUE);

					if (Safety_value[i] <= Tension_value[i] && Safety_mode[i] == 0) {//�Z���T�̒l��臒l�𒴂����炻�̎��̃G���R�[�_���ő�l��
						VCS_MoveToPosition(keyHandle, NodeId + i, ActualValue[i], 1, 1, &errorCode);
						Max_value[i] = ActualValue[i];
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETRANGEMAX, 0, Max_value[i] / -256);
						SetDlgItemInt(hwnd, IDC_EDIT11 + i, Max_value[i] / -256, TRUE);
						SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_SETPOS, TRUE, Max_value[i] / -256);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
						Safety_mode[i] = 1;
					}

					if (Safety_value[i] >= Tension_value[i]) {
						Safety_mode[i] = 0;
					}
				}
			}
			return 1;

		case 2://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home1), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection1), 1);
				KillTimer(hwnd, 2);
			}
			return 1;
		case 3://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 1, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home2), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection2), 1);
				KillTimer(hwnd, 3);
			}
			return 1;
		case 4://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 2, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home3), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection3), 1);
				KillTimer(hwnd, 4);
			}
			return 1;
		case 5://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 3, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home4), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection4), 1);
				KillTimer(hwnd, 5);
			}
			return 1;
		case 6://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 4, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home5), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection5), 1);
				KillTimer(hwnd, 6);
			}
			return 1;
		case 7://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 5, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home6), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection6), 1);
				KillTimer(hwnd, 7);
			}
			return 1;
		case 8://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 6, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home7), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection7), 1);
				KillTimer(hwnd, 8);
			}
			return 1;
		case 9://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 7, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home8), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection8), 1);
				KillTimer(hwnd, 9);
			}
			return 1;
		case 10://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 8, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home9), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection9), 1);
				KillTimer(hwnd, 10);
			}
			return 1;
		case 11://�����ʒu(�G���R�[�_��0)�ɂȂ���������
			if (VCS_WaitForTargetReached(keyHandle, NodeId + 9, 1, &errorCode) == 1) {
				EnableWindow(GetDlgItem(hwnd, IDC_Home10), 0);
				EnableWindow(GetDlgItem(hwnd, IDC_Connection10), 1);
				KillTimer(hwnd, 11);
			}
			return 1;

			/*case 12:
				VCS_MoveToPosition(keyHandle, NodeId , a * -256, 1, 1, &errorCode);
				return 1;*/
		}
		// �����ŏ������Ȃ��R���g���[�����I�����ꂽ�ꍇ��Windows�ɔC���邽�߂�0��Ԃ��B 
		return 0;

		//����������
	case WM_INITDIALOG:
		InitCommonControls();
		SetTimer(hwnd, 1, 100, NULL);
		for (i = 0; i <= 9; i++) {
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETRANGE, TRUE, MAKELPARAM(0, 190)); //�X���C�_�[�R���g���[���̃����W���w��
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETTICFREQ, 1, 0);   // �X���C�_�[�R���g���[���̖ڐ���̑���
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETPAGESIZE, 0, 1); // �X���C�_�[�R���g���[���̃N���b�N���̈ړ���
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_SETRANGE, TRUE, MAKELPARAM(0, 190)); //�X���C�_�[�R���g���[���̃����W���w��
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_SETTICFREQ, 1, 0);   // �X���C�_�[�R���g���[���̖ڐ���̑���
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_SETPOS, TRUE, Max_value[i] / -256);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_SETPAGESIZE, 0, 1); // �X���C�_�[�R���g���[���̃N���b�N���̈ړ���
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER21 + i), TBM_SETRANGE, TRUE, MAKELPARAM(0, 5000)); //�X���C�_�[�R���g���[���̃����W���w��
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER21 + i), TBM_SETTICFREQ, 1, 0);   // �X���C�_�[�R���g���[���̖ڐ���̑���
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER21 + i), TBM_SETPOS, TRUE, Safety_value[i]);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			SendMessage(GetDlgItem(hwnd, IDC_SLIDER21 + i), TBM_SETPAGESIZE, 0, 1); // �X���C�_�[�R���g���[���̃N���b�N���̈ړ���
			EnableWindow(GetDlgItem(hwnd, IDC_Connection1 + i), 0);//�ڑ��𖳌�
			EnableWindow(GetDlgItem(hwnd, IDC_Disconnection1 + i), 0);//�ؒf�𖳌�
			EnableWindow(GetDlgItem(hwnd, IDC_RESET1 + i), 0);//���ݒn0�{�^���𖳌�
			SetDlgItemInt(hwnd, IDC_EDIT11 + i, Max_value[i] / -256, TRUE);//�ő�l�̏�����
			SetDlgItemInt(hwnd, IDC_EDIT31 + i, Safety_value[i], TRUE);//�ő�l�̏�����
		}
		return 1;

	case WM_HSCROLL://�X���C�_�[�R���g���[���𓮂������Ƃ�
		for (i = 0; i < 10; i++) {
			if (GetDlgItem(hwnd, IDC_SLIDER1 + i) == (HWND)lParam)
			{
				Slider_Value = SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_GETPOS, NULL, NULL); // �X���C�_�[�R���g���[���̌��݂̒l�̎擾
				VCS_MoveToPosition(keyHandle, NodeId + i, Slider_Value * -256, 1, 1, &errorCode);
			}
			if (GetDlgItem(hwnd, IDC_SLIDER11 + i) == (HWND)lParam)
			{
				Max_value[i] = SendMessage(GetDlgItem(hwnd, IDC_SLIDER11 + i), TBM_GETPOS, NULL, NULL) * -256; // �X���C�_�[�R���g���[���̌��݂̒l�̎擾
				SetDlgItemInt(hwnd, IDC_EDIT11 + i, Max_value[i] / -256, TRUE);
				SendMessage(GetDlgItem(hwnd, IDC_SLIDER1 + i), TBM_SETRANGEMAX, 1, Max_value[i] / -256);
			}
			if (GetDlgItem(hwnd, IDC_SLIDER21 + i) == (HWND)lParam)
			{
				Safety_value[i] = SendMessage(GetDlgItem(hwnd, IDC_SLIDER21 + i), TBM_GETPOS, NULL, NULL); // �X���C�_�[�R���g���[���̌��݂̒l�̎擾
				SetDlgItemInt(hwnd, IDC_EDIT31 + i, Safety_value[i], TRUE);
			}
		}
		break;

	case WM_CLOSE:		// �~�{�^�����N���b�N�����Ƃ��̏��� 
		SetTimer(start, 1, 100, NULL);
		KillTimer(hwnd, 1);
		DestroyWindow(hwnd); 		//�_�C�A���O��j�� 
		return 1;

	}
	// �����ŏ������Ȃ��E�B���h�E�̃��b�Z�[�W�� Windows �ɔC���邽�� 0 ��Ԃ��B
	return 0;
}



//���n�r���ڍאݒ�
BOOL CALLBACK DialogFunc1(HWND hwndR, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD ID;

	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Plus1:
			R_Target++;
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Plus10:
			R_Target = R_Target + 10;
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Minus1:
			R_Target--;
			if (R_Target <= 0) { R_Target = 1; }
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Minus10:
			R_Target = R_Target - 10;
			if (R_Target <= 0) { R_Target = 1; }
			SetDlgItemInt(hwndR, IDC_Rehabili_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_FlexionData://�t�@�C���I�[�v��
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Flexion_value[k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Flexion_value[k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}
			if (Read_flag == 1) {
				EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
				Read_flag = 0;
			}
			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Fle_value1 + i, Flexion_value[i] / -256, TRUE);//
			}
			return 1;

		case IDC_ExtensionData:
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Extension_value[k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Extension_value[k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}
			if (Read_flag == 1) {
				EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 1);
				Read_flag = 0;
			}
			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Ext_value1 + i, Extension_value[i] / -256, TRUE);//
			}
			return 1;




		case IDC_Rotation_a:
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Rotation_value[0][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Rotation_value[0][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}

			Read_flag_R++;//4�f�[�^�ǂݍ��񂾂�

			if (Read_flag_R == 4) {
				EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
				Read_flag_R = 0;
			}

			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Rotation_value1 + i, Rotation_value[0][i] / -256, TRUE);
			}
			return 1;


		case IDC_Rotation_b:
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Rotation_value[1][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Rotation_value[1][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}


			Read_flag_R++;

			if (Read_flag_R == 4) {
				EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
				Read_flag_R = 0;
			}

			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Rotation_value11 + i, Rotation_value[1][i] / -256, TRUE);
			}
			return 1;

		case IDC_Rotation_c:
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Rotation_value[2][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Rotation_value[2][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}


			Read_flag_R++;

			if (Read_flag_R == 4) {
				EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
				Read_flag_R = 0;
			}

			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Rotation_value21 + i, Rotation_value[2][i] / -256, TRUE);
			}
			return 1;

		case IDC_Rotation_d:
			funcFileOpen(hwndR);
			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Rotation_value[3][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Rotation_value[3][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}

			Read_flag_R++;

			if (Read_flag_R == 4) {
				EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 1);
				Read_flag_R = 0;
			}

			for (i = 0; i < 10; i++) {
				SetDlgItemInt(hwndR, Rotation_value31 + i, Rotation_value[3][i] / -256, TRUE);
			}
			return 1;


		case IDC_NormalReha://���ȁ@�L�W
			EnableWindow(GetDlgItem(hwndR, IDC_FlexionData), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_ExtensionData), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_NormalReha), 0);
			//�����ʒu�Ɏw�𓮂���
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					Reach_check[i] = 0;
					VCS_MoveToPosition(keyHandle, NodeId + i, Extension_value[i], 1, 0, &errorCode);
				}
			}
			Reha_end = 0;
			Reha_close = 1;
			CreateThread(NULL, 0, Normal_Reha_Thread, 0, 0, &ID);//���s�����œ���
			return 1;


		case IDC_RotationReha://�d�w��]
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_a), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_b), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_c), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_Rotation_d), 0);
			EnableWindow(GetDlgItem(hwndR, IDC_RotationReha), 0);
			//�����ʒu(a)�Ɏw�𓮂���
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					Reach_check[i] = 0;
					VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_value[0][i], 1, 0, &errorCode);
				}
			}
			Reha_end = 0;
			Reha_close = 1;
			CreateThread(NULL, 0, Rotation_Reha_Thread, 0, 0, &ID);//���s�����œ���
			return 1;



		case IDC_Rehabili_CANCEL:
			Reha_end = 1;
			Now_position = 0;
			Next_position = 1;
			return 1;
		}
		return 0;

		//����������
	case WM_INITDIALOG:
		SetDlgItemInt(hwndR, IDC_Rehabili_EDIT1, R_Target, TRUE);
		SetDlgItemInt(hwndR, IDC_Rehabili_EDIT2, 0, TRUE);
		for (i = 0; i < 10; i++) {
			SetDlgItemInt(hwndR, Reha_th1 + i, Safety_value[i], TRUE);
		}
		return 1;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE://���n�r�����͂ł��Ȃ�����
		if (Reha_close == 0) {
			SetTimer(hwnd, 1, 100, NULL);
			DestroyWindow(hwndR);
		}
		else {
			MessageBox(hwndR, "���n�r�����I�����Ă��܂���B", "�x��", MB_OK);
		}
		return 1;

	}
	return 0;
}

BOOL CALLBACK DialogSTART(HWND strat, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_RADIOGUF://�O�[�p
			h = 0;
			e = 0;
			return 1;

		case IDC_RADIOPIF://�s���`
			h = 1;
			e = 0;
			return 1;

		case IDC_RADIOZF://���w�L�W
			h = 2;
			e = 0;
			return 1;

		case IDC_RADIORF://���]
			h = 3;
			e = 0;
			return 1;

		case IDC_RADIOPA:
			h = 0;
			e = 1;
			return 1;

		case IDC_RADIOPI:
			h = 1;
			e = 1;
			return 1;

		case IDC_RADIOGA:
			h = 3;
			e = 1;
			return 1;

		case IDC_RADIORATE:
			h = 4;
			e = 0;

			return 1;

		case IDC_RADIORATEK:
			h = 4;
			e = 1;

			return 1;

		case IDC_RADIOHARA:
			h = 5;
			e = 0;

			return 1;

		case IDC_RADIOTHQ:
			h = 6;
			e = 0;
			return 1;

		case IDC_RADIOTHEX:
			h = 6;
			e = 1;
			return 1;

		case IDC_RADIOa:
			r = 0;
			return 1;

		case IDC_RADIOb:
			r = 1;
			return 1;

		case IDC_RADIOc:
			r = 2;
			return 1;



		case IDC_Start:
			SetDlgItemText(strat, IDC_Start_Edit, "��������");
			SetDlgItemText(strat, IDC_Fle_Edit, "���ݒ�");
			SetDlgItemText(strat, IDC_Ext_Edit, "���ݒ�");
			SetDlgItemText(strat, IDC_Rotation_Edit1, "���ݒ�");


			SetTimer(strat, 2, 100, NULL);
			for (i = 0; i <= 9; i++) {
				Flexion_Value[0][i] = 0;
				Flexion_Value[1][i] = 0;
				Flexion_Value[2][i] = 0;
				Flexion_Value[3][i] = 0;
				Flexion_Value[4][i] = 0;
				Flexion_Value[5][i] = 0;
				Extension_Value[0][i] = 0;
				Extension_Value[1][i] = 0;
				Extension_Value[2][i] = 0;
				Extension_Value[3][i] = 0;
				Extension_Value[4][i] = 0;
				Extension_Value[5][i] = 0;
				Rotation_Value[0][i] = 0;
				Rotation_Value[1][i] = 0;
				Rotation_Value[2][i] = 0;
				Rotation_Value[3][i] = 0;
				Rotation_Value[4][i] = 0;

				VCS_ClearFault(keyHandle, NodeId + i, &errorCode);

				SetDlgItemText(strat, IDC_Start_Edit, "��������1");

				if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
				{
					if (VCS_SetHomingParameter(keyHandle, NodeId + i, 10000, 1000, 1, 0, 500, 0, &errorCode))
					{
						if (VCS_SetEnableState(keyHandle, NodeId + i, &errorCode))
						{
							VCS_FindHome(keyHandle, NodeId + i, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
							SetDlgItemText(strat, IDC_Start_Edit, "��������2");
						}
						SetDlgItemText(strat, IDC_Start_Edit, "��������3");
					}
					SetDlgItemText(strat, IDC_Start_Edit, "��������4");
				}
				SetDlgItemText(strat, IDC_Start_Edit, "��������5");
			}
			return 1;


		case IDC_Start_Fle1://�f�[�^�ǂݏo��
			funcFileOpen(strat);


			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					if (e == 0)Flexion_Value[h][k] = atoi(tmp);
					if (e == 1)Extension_Value[h][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					if (e == 0)Flexion_Value[h][k] = atoi(tmp);
					if (e == 1)Extension_Value[h][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}
			if (Read_flag == 1) {
				Read_flag = 0;//�ڍאݒ�̕��Ŏg������0�ɂ��Ă���
			}
			//������
			if (e == 0)
			{
				for (i = 0; i <= 9; i++) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[h][i], 1, 0, &errorCode);
				}
			}

			if (e == 1)
			{
				for (i = 0; i <= 9; i++) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[h][i], 1, 0, &errorCode);
				}
			}

			if (Flexion_Value[h][0] == 0 && Flexion_Value[h][1] == 0 && Flexion_Value[h][2] == 0 && Flexion_Value[h][3] == 0 && Flexion_Value[h][4] == 0 && Flexion_Value[h][5] == 0 && Flexion_Value[h][6] == 0 && Flexion_Value[h][7] == 0 && Flexion_Value[h][8] == 0 && Flexion_Value[h][9] == 0) {
				SetDlgItemText(strat, IDC_Fle_Edit, "���ݒ�");
			}
			else {
				SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ芮��");
			}

			if (Extension_Value[h][0] == 0 && Extension_Value[h][1] == 0 && Extension_Value[h][2] == 0 && Extension_Value[h][3] == 0 && Extension_Value[h][4] == 0 && Extension_Value[h][5] == 0 && Extension_Value[h][6] == 0 && Extension_Value[h][7] == 0 && Extension_Value[h][8] == 0 && Extension_Value[h][9] == 0) {
				SetDlgItemText(strat, IDC_Ext_Edit, "���ݒ�");
			}
			else {

				SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ芮��");
			}

			return 1;

		case IDC_Start_Fle2://�ɂ߂�
			for (i = 0; i <= 9; i++) {
				VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
			}

			for (i = 4; i <= 22; i++) {
				KillTimer(strat, i);
			}
			hara = 0;
			SetTimer(strat, 1, 100, NULL);
			return 1;

		case IDC_Start_Fle3://�ۑ�
			for (i = 0; i < 10; i++) {
				VCS_GetPositionIs(keyHandle, NodeId + i, &ActualValue[i], &errorCode);//���݂̃G���R�[�_�̒l���擾

				if (e == 0)
				{
					Flexion_Value[h][i] = ActualValue[i];//�����Flexion_value���Q�Ƃ��čs�����ߕۑ����� �O�[
					Save_value[i] = Flexion_Value[h][i];
				}
				if (e == 1)
				{

					Extension_Value[h][i] = ActualValue[i];//�����Extension_value���Q�Ƃ��čs�����ߕۑ�����
					Save_value[i] = Extension_Value[h][i];
				}
			}
			sprintf_s(Save_Buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", Save_value[0], Save_value[1], Save_value[2], Save_value[3], Save_value[4], Save_value[5], Save_value[6], Save_value[7], Save_value[8], Save_value[9]);
			funcFileSave(strat);
			for (i = 0; i <= 9; i++) {//�����ɂ߂�
				VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
			}
			hara = 0;
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ芮��");
			return 1;



		case IDC_Start_Rotation1://�f�[�^�ǂݏo��
			funcFileOpen(strat);

			for (i = 0;; i++) {
				tmp[j] = Read_Buffer[i];
				j++;
				if (Read_Buffer[i] == ',') {
					Rotation_Value[r][k] = atoi(tmp);
					k++;
					j = 0;
				}
				else if (Read_Buffer[i] == NULL) {
					Rotation_Value[r][k] = atoi(tmp);
					k = 0;
					j = 0;
					break;
				}
			}
			if (Read_flag == 1) {
				Read_flag = 0;//�ڍאݒ�̕��Ŏg������0�ɂ��Ă���
			}
			//������
			for (i = 0; i <= 9; i++) {
				VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[r][i], 1, 1, &errorCode);
			}
			if (Rotation_Value[r][0] == 0 && Rotation_Value[r][1] == 0 && Rotation_Value[r][2] == 0 && Rotation_Value[r][3] == 0 && Rotation_Value[r][4] == 0 && Rotation_Value[r][5] == 0 && Rotation_Value[r][6] == 0 && Rotation_Value[r][7] == 0 && Rotation_Value[r][8] == 0 && Rotation_Value[r][9] == 0) {
				SetDlgItemText(strat, IDC_Rotation_Edit1, "���ݒ�");
			}
			else {
				SetDlgItemText(strat, IDC_Rotation_Edit1, "�ݒ芮��");
			}
			return 1;


		case IDC_Start_Rotation3://�ۑ�

			for (i = 0; i < 10; i++) {
				VCS_GetPositionIs(keyHandle, NodeId + i, &ActualValue[i], &errorCode);//���݂̃G���R�[�_�̒l���擾

				Rotation_Value[r][i] = ActualValue[i];//�����Extension_value���Q�Ƃ��čs�����ߕۑ�����
				Save_value[i] = Rotation_Value[r][i];

			}

			sprintf_s(Save_Buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", Save_value[0], Save_value[1], Save_value[2], Save_value[3], Save_value[4], Save_value[5], Save_value[6], Save_value[7], Save_value[8], Save_value[9]);
			funcFileSave(strat);
			for (i = 0; i <= 9; i++) {//�����ɂ߂�
				VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
			}
			SetDlgItemText(strat, IDC_Rotation_Edit1, "�ݒ芮��");
			return 1;

		case IDC_Fle_Setting1://�S�w����
			KillTimer(strat, 1);//
			h = 0;
			e = 0;
			SetTimer(strat, 8, 16, NULL);//�d�w���Ȕ���
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 15, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[4] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &AD_TMP[4], &errorCode);//�d�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 4, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 4, -38400, 1, 1, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);//���w����
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);//3�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);//15cm���炢����
			}
			return 1;



		case IDC_Fle_Setting3://�w��܂�
			KillTimer(strat, 1);//
			h = 1;
			e = 0;
			SetTimer(strat, 6, 16, NULL);//�d�w�Η�����
			SetTimer(strat, 8, 16, NULL);//�d�w���Ȕ���
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			//SetTimer(strat, 13, 16, NULL);//3�w�L�W����
			SetTimer(strat, 19, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[2] == 1 && Connection_Check[4] == 1 && Connection_Check[5] == 1 /*&& Connection_Check[9] == 1*/) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &AD_TMP[2], &errorCode);//�d�w�Η�
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 2, -38400, 1, 0, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &AD_TMP[4], &errorCode);//�d�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 4, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 4, -38400, 1, 0, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);//���w����
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 0, &errorCode);//15cm���炢����
				/*VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &AD_TMP[9], &errorCode);//3�w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 9, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 9, -38400, 1, 0, &errorCode);//15cm���炢����
				*/
			}
			return 1;

		case IDC_Fle_Setting4://�d�w�̓��]
			KillTimer(strat, 1);//
			h = 3;
			e = 0;
			SetTimer(strat, 4, 16, NULL);//�d�w���]����
			//SetTimer(strat, 6, 16, NULL);//�d�w�Η�����
			//SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			//SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			//SetTimer(strat, 11, 16, NULL);//�d�w�L�W����
			SetTimer(strat, 21, 16, NULL);//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[0] == 1 /*&& Connection_Check[2] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1 && Connection_Check[7] == 1*/) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 0, 1, &AD_TMP[0], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 0, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 0, -38400, 1, 1, &errorCode);
				/*	VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &AD_TMP[2], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 2, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 2, -38400, 1, 1, &errorCode);
					VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);
					VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);
					VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &AD_TMP[7], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 7, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 7, -38400, 1, 1, &errorCode);
					*/
			}
			return 1;

		case IDC_Ext_Setting1://�O�[�̐L�W
			KillTimer(strat, 1);//
			h = 0;
			e = 1;
			SetTimer(strat, 11, 16, NULL);//�d�w�L�W����
			SetTimer(strat, 12, 16, NULL);//���w�L�W����
			SetTimer(strat, 13, 16, NULL);//3�w�L�W����
			SetTimer(strat, 16, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[7] == 1 && Connection_Check[8] == 1 && Connection_Check[9] == 1) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &AD_TMP[7], &errorCode);//�d�w�L�W�@VCS_GetAnalogInput;�A�i���O���͂���l��Ԃ�
				VCS_SetPositionProfile(keyHandle, NodeId + 7, 500, 500, 500, &errorCode);//VCS_SetPositionProfile;�ʒu�v���t�@�C���p�����[�^��ݒ�
				VCS_MoveToPosition(keyHandle, NodeId + 7, -38400, 1, 0, &errorCode);//15cm���炢����

				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &AD_TMP[8], &errorCode);//���w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 8, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 8, -38400, 1, 0, &errorCode);//15cm���炢����

				VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &AD_TMP[9], &errorCode);//3�w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 9, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 9, -38400, 1, 0, &errorCode);//15cm���炢����
			}
			return 1;

		case IDC_Ext_Setting2://���w�̐L�W
			KillTimer(strat, 1);//
			h = 2;
			SetTimer(strat, 8, 16, NULL);//�d�w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 12, 16, NULL);//���w�L�W����
			SetTimer(strat, 18, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[4] == 1 && Connection_Check[6] == 1 && Connection_Check[8] == 1) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &AD_TMP[4], &errorCode);//�d�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 4, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 4, -38400, 1, 0, &errorCode);//15cm���炢����

				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &AD_TMP[8], &errorCode);//���w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 8, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 8, -38400, 1, 1, &errorCode);//15cm���炢����
			}
			return 1;

		case IDC_Ext_Setting3://�s���`����
			KillTimer(strat, 1);//
			h = 1;
			e = 1;
			SetTimer(strat, 7, 16, NULL);//�d�w�Η���������
			SetTimer(strat, 11, 16, NULL);//�d�w�L�W����
			SetTimer(strat, 12, 16, NULL);//���w�L�W����
		//	SetTimer(strat, 13, 16, NULL);//3�w�L�W����
			SetTimer(strat, 20, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[3] == 1 && Connection_Check[7] == 1 && Connection_Check[8] == 1 /*&& Connection_Check[9] == 1*/) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &AD_TMP[3], &errorCode);//�d�w�Η�����
				VCS_SetPositionProfile(keyHandle, NodeId + 3, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 3, -38400, 1, 0, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &AD_TMP[7], &errorCode);//�d�w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 7, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 7, -38400, 1, 0, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &AD_TMP[8], &errorCode);//���w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 8, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 8, -38400, 1, 0, &errorCode);//15cm���炢����
				/*VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &AD_TMP[9], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 9, 500, 500, 500, &errorCode);//3�w�L�W
				VCS_MoveToPosition(keyHandle, NodeId + 9, -38400, 1, 0, &errorCode);
				*/
			}
			return 1;

		case IDC_RATE: //���e�����s���`

			KillTimer(strat, 1);
			h = 4;
			e = 0;
			SetTimer(strat, 6, 16, NULL);//�d�w�Η�����
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 4, 16, NULL);//�d�w���]����
			SetTimer(strat, 25, 16, NULL);//�w�p������
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[0] == 1 && Connection_Check[2] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId + 0, 1, &AD_TMP[0], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 0, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 0, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &AD_TMP[2], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 2, -38400, 1, 0, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);
			}

			return 1;

		case IDC_RATEK://���e��������

			KillTimer(strat, 1);//
			h = 4;
			e = 1;
			SetTimer(strat, 5, 16, NULL);//�d�w�O�]����
			SetTimer(strat, 7, 16, NULL);//�d�w�Η���������
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 22, 16, NULL);//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[1] == 1 && Connection_Check[3] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 1, 1, &AD_TMP[1], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 1, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 1, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &AD_TMP[3], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 3, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 3, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);

			}

			return 1;



		case IDC_HARA://�w���܂�

			KillTimer(strat, 1);
			h = 5;
			e = 0;
			hara = 1;

			SetTimer(strat, 12, 16, NULL);//���w�L�W����
			SetTimer(strat, 6, 16, NULL);//�d�w�Η�����
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
		//	SetTimer(strat, 13, 16, NULL);//3�w�L�W����
			SetTimer(strat, 27, 16, NULL);//�w�p���̔���

			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[2] = 1 && Connection_Check[5] == 1/*&& Connection_Check[9] == 1*/ && Connection_Check[8] == 1) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &AD_TMP[2], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 2, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &AD_TMP[8], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 8, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 8, -38400, 1, 1, &errorCode);

				/*	VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &AD_TMP[9], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 9, 500, 500, 500, &errorCode);//3�w�L�W
					VCS_MoveToPosition(keyHandle, NodeId + 9, -38400, 1, 0, &errorCode);
					*/



			}

			return 1;

		case IDC_Ext_Setting4://�d�w�̊O�]
			KillTimer(strat, 1);//
			h = 3;
			e = 1;
			SetTimer(strat, 5, 16, NULL);//�d�w�O�]����
		//	SetTimer(strat, 7, 16, NULL);//�d�w�Η���������
		//	SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
		//	SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 22, 16, NULL);//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[1] == 1 /*&& Connection_Check[3] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1*/) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 1, 1, &AD_TMP[1], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 1, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 1, -38400, 1, 1, &errorCode);

				/*	VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &AD_TMP[3], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 3, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 3, -38400, 1, 1, &errorCode);

					VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

					VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
					VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
					VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);
					*/
			}
			return 1;

		case IDC_Rotation_Setting1://��w��] ���]�L�W
			KillTimer(strat, 1);//
			r = 0;
			SetTimer(strat, 4, 16, NULL);//��w���]
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
		//	SetTimer(strat, 11, 16, NULL);//��w�L�W����
			SetTimer(strat, 23, 16, NULL);//�w�p������
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[0] = 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1 /*&& Connection_Check[7] == 1*/) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId, 1, &AD_TMP[0], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId, -38400, 1, 0, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);


				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);


			}
			return 1;

		case IDC_Rotation_Setting2://��w��]b�@�Η������A�O�]
			KillTimer(strat, 1);
			r = 1;
			SetTimer(strat, 5, 16, NULL);//�O�]����
			SetTimer(strat, 7, 16, NULL);//�Η���������
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 24, 16, NULL);//�w�p������
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[1] = 1 && Connection_Check[3] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId + 1, 1, &AD_TMP[1], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 1, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 1, -38400, 1, 0, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &AD_TMP[3], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 3, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 3, -38400, 1, 0, &errorCode);


				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);
			}
			return 1;

		case IDC_Rotation_Setting3://��w��]c �Η�
			KillTimer(strat, 1);
			r = 2;
			SetTimer(strat, 6, 16, NULL);//�d�w�Η�����
			SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 25, 16, NULL);//�w�p������
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[2] = 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1) {//�K�v�Ȏw�𓮂���

				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &AD_TMP[2], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 2, -38400, 1, 0, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);

				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);
			}
			return 1;

		case IDC_THQ:
			KillTimer(strat, 1);//
			h = 6;
			e = 0;
			SetTimer(strat, 8, 16, NULL);//�d�w���Ȕ���
			//SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
			//SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 28, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (Connection_Check[4] == 1/* && Connection_Check[5] == 1 && Connection_Check[6] == 1*/) {//�K�v�Ȏw�𓮂���
				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &AD_TMP[4], &errorCode);//�d�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 4, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 4, -38400, 1, 1, &errorCode);//15cm���炢����
				/*VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);//���w����
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);//3�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);//15cm���炢����
				*/
			}
			return 1;


			return 1;

		case IDC_THEX://�d�w�L�W
			KillTimer(strat, 1);//
			h = 6;
			e = 1;
			SetTimer(strat, 11, 16, NULL);//�d�w�L�W����
		//	SetTimer(strat, 9, 16, NULL);//���w���Ȕ���
		//	SetTimer(strat, 10, 16, NULL);//3�w���Ȕ���
			SetTimer(strat, 26, 16, NULL);	//�w�p���̔���
			SetDlgItemText(strat, IDC_Fle_Edit, "�ݒ蒆");
			Auto_setting = 0;
			for (i = 0; i <= 9; i++) {//�I�[�g�ݒ�I���̔����������
				Auto_check[i] = 0;
			}
			if (/*Connection_Check[5] == 1 && Connection_Check[6] == 1&&*/Connection_Check[7] == 1) {//�K�v�Ȏw�𓮂���

				/*VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &AD_TMP[5], &errorCode);//���w����
				VCS_SetPositionProfile(keyHandle, NodeId + 5, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 5, -38400, 1, 1, &errorCode);//15cm���炢����
				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &AD_TMP[6], &errorCode);//3�w����
				VCS_SetPositionProfile(keyHandle, NodeId + 6, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 6, -38400, 1, 1, &errorCode);//15cm���炢����
				*/
				VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &AD_TMP[7], &errorCode);//�d�w�L�W
				VCS_SetPositionProfile(keyHandle, NodeId + 7, 500, 500, 500, &errorCode);
				VCS_MoveToPosition(keyHandle, NodeId + 7, -38400, 1, 0, &errorCode);//15cm���炢����

			}

			return 1;


		case IDC_Start_Reha:
			KillTimer(strat, 1);
			// (1)���\�[�X�Ɋ�Â��_�C�A���O�{�b�N�X�쐬
			hwndSR = CreateDialog(simplereha, "SIMPLEREHA", NULL, (DLGPROC)DialogSIMPLEREHA);
			// �\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndSR == NULL)
			{
				MessageBox(NULL, "���s�ł��܂���", "�I��", MB_OK);
				return 0;
			}
			// (2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndSR, SW_SHOW);
			UpdateWindow(hwndSR);
			return 1;

		case IDC_Start_Sensor://�Z���T�[����
			KillTimer(strat, 1);
			// (1)���\�[�X�Ɋ�Â��_�C�A���O�{�b�N�X�쐬
			hwndS = CreateDialog(sensor, "GRAPHCLASS_SAMPLE3", NULL, (DLGPROC)DialogS);
			// �\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndS == NULL)
			{
				MessageBox(NULL, "���s�ł��܂���", "�I��", MB_OK);
				return 0;
			}
			// (2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndS, SW_SHOW);
			UpdateWindow(hwndS);
			return 1;

		case IDC_Start_Setting:
			KillTimer(strat, 1);
			// (1)���\�[�X�Ɋ�Â��_�C�A���O�{�b�N�X�쐬
			hwnd = CreateDialog(home, "HOME", NULL, (DLGPROC)DialogFunc);
			// �\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwnd == NULL)
			{
				MessageBox(NULL, "���s�ł��܂���", "�I��", MB_OK);
				return 0;
			}
			// (2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
			return 1;

		case IDC_Start_End:
			SetDlgItemText(strat, IDC_End_Edit, "�I��������");
			SetTimer(strat, 3, 100, NULL);
			for (i = 0; i < 10; i++) {
				VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
			}
			return 1;

		case IDC_Start_Leap:
			KillTimer(strat, 1);
			// (1)���\�[�X�Ɋ�Â��_�C�A���O�{�b�N�X�쐬
			hwndL = CreateDialog(leap, "LeapMotion", NULL, (DLGPROC)DialogLeap);
			// �\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndL == NULL)
			{
				MessageBox(NULL, "���s�ł��܂���", "�I��", MB_OK);
				return 0;
			}
			// (2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndL, SW_SHOW);
			UpdateWindow(hwndL);
			return 1;

		}

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 1://�G���R�[�_��AD�l�̎擾
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_GetPositionIs(keyHandle, NodeId + i, &ActualValue[i], &errorCode);//���݂̃G���R�[�_�̒l���擾
					if (Max_value[i] / 256 > ActualValue[i] / 256) {//�ő�l���G���R�[�_���������烂�[�^�[��0�� 
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);//VCS_MoveToPosition �ڕW�ʒu�܂ňړ��@3�Ԗ�(0)���ڕW�ʒu
					}
					VCS_GetAnalogInput(keyHandle, NodeId + i, 1, &Tension_value[i], &errorCode);//�����̓Z���T�̌��݂̒l
					Tension_value[i] = (Tension_value[i] + Before_Tension_value[i] + Before_Tension_value2[i]) / 3;//�ړ�����
					Before_Tension_value2[i] = Before_Tension_value[i];//�ړ�����
					Before_Tension_value[i] = Tension_value[i];//�ړ�����
					if (Safety_value[i] <= Tension_value[i]) {
						SetDlgItemText(strat, IDC_Start_AD1 + i, "���ɂ݁�");
					}
					else {
						SetDlgItemInt(strat, IDC_Start_AD1 + i, Tension_value[i], TRUE);
					}
				}
			}
			return 1;

		case 2://����������
			for (i = 0; i <= 9; i++) {
				if (Connection_Check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {//��Ԃ��^�[�Q�b�g�ɓ��B����悤�ɕύX�����܂ŁA�܂��͎��Ԃ��o�߂���܂őҋ@
					Connection_Check[i] = 1;
					if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_PROFILE_POSITION_MODE, &errorCode))
					{
						if (VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode))//�v���t�@�C���ʒu���[�h
						{
							VCS_SetEnableState(keyHandle, NodeId + i, &errorCode);//�f�o�C�X��L���ɂ���
						}
					}
				}
			}
			SetDlgItemText(strat, IDC_Start_Edit, "��������11");
			if (Connection_Check[0] == 1 && Connection_Check[1] == 1 && Connection_Check[2] == 1 && Connection_Check[3] == 1 && Connection_Check[4] == 1 && Connection_Check[5] == 1 && Connection_Check[6] == 1 && Connection_Check[7] == 1 && Connection_Check[8] == 1 && Connection_Check[9] == 1) {
				SetTimer(strat, 1, 100, NULL);
				SetDlgItemText(strat, IDC_Start_Edit, "����");
				EnableWindow(GetDlgItem(strat, IDC_Start), 0);
				KillTimer(strat, 2);
			}

			else if (Connection_Check[2] == 0 && Connection_Check[3] == 0) {
				SetDlgItemText(strat, IDC_Start_Edit, "������");
			}

			return 1;

		case 3://�I������
			for (i = 0; i <= 9; i++) {
				if (Connection_Check[i] == 1 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 1, &errorCode)) {
					Connection_Check[i] = 0;
				}
			}
			if (Connection_Check[0] == 0 && Connection_Check[1] == 0 && Connection_Check[2] == 0 && Connection_Check[3] == 0 && Connection_Check[4] == 0 && Connection_Check[5] == 0 && Connection_Check[6] == 0 && Connection_Check[7] == 0 && Connection_Check[8] == 0 && Connection_Check[9] == 0) {//�������I�������
				KillTimer(strat, 1);
				for (i = 0; i <= 9; i++) {//���ׂĐڑ��؂�
					VCS_SetDisableState(keyHandle, NodeId + i, &errorCode);//�f�o�C�X�𖳌��ɂ���
				}
				SetDlgItemText(strat, IDC_End_Edit, "�����l�ł����B�~�{�^�����N���b�N���Ă��������B");
				EnableWindow(GetDlgItem(strat, IDC_Start), 1);
				KillTimer(strat, 3);
			}
			return 1;

			///�e�w�̔���///
		case 4://0:�d�w���]�̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId, 1, &Tension_value[0], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[0] = (Tension_value[0] + Before_Tension_value[0] + Before_Tension_value2[0]) / 3;//�ړ�����
				Before_Tension_value2[0] = Before_Tension_value[0];//�ړ�����
				Before_Tension_value[0] = Tension_value[0];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1, Tension_value[0], TRUE);
				if (Safety_value[0] <= Tension_value[0]) {
					VCS_MoveToPosition(keyHandle, NodeId, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[0] - AD_TMP[0] >= 650 && Auto_check[0] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId, &errorCode);
					Auto_check[0] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 1, 1, &Tension_value[1], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[1] = (Tension_value[1] + Before_Tension_value[1] + Before_Tension_value2[1]) / 3;//�ړ�����
				Before_Tension_value2[1] = Before_Tension_value[1];//�ړ�����
				Before_Tension_value[1] = Tension_value[1];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 1, Tension_value[1], TRUE);
				if (Safety_value[1] <= Tension_value[1]) {
					VCS_MoveToPosition(keyHandle, NodeId + 1, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[1] - AD_TMP[1] >= 20 && Auto_check[1] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 1, 512, 0, 1, &errorCode);////////
					Auto_check[1] = 1;
					KillTimer(strat, 4);
				}
			}
			return 1;

		case 5://1:�d�w�O�]�̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 1, 1, &Tension_value[1], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[1] = (Tension_value[1] + Before_Tension_value[1] + Before_Tension_value2[1]) / 3;//�ړ�����
				Before_Tension_value2[1] = Before_Tension_value[1];//�ړ�����
				Before_Tension_value[1] = Tension_value[1];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 1, Tension_value[1], TRUE);
				if (Safety_value[1] <= Tension_value[1]) {
					VCS_MoveToPosition(keyHandle, NodeId + 1, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[1] - AD_TMP[1] >= 1100 && Auto_check[1] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 1, &errorCode);
					Auto_check[1] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId, 1, &Tension_value[0], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[0] = (Tension_value[0] + Before_Tension_value[0] + Before_Tension_value2[0]) / 3;//�ړ�����
				Before_Tension_value2[0] = Before_Tension_value[0];//�ړ�����
				Before_Tension_value[0] = Tension_value[0];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1, Tension_value[0], TRUE);
				if (Safety_value[0] <= Tension_value[0]) {
					VCS_MoveToPosition(keyHandle, NodeId, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[0] - AD_TMP[0] >= 20 && Auto_check[0] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId, 256, 0, 1, &errorCode);////////
					Auto_check[0] = 1;
					KillTimer(strat, 5);
				}
			}
			return 1;

		case 6://2:�d�w�Η��̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &Tension_value[2], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[2] = (Tension_value[2] + Before_Tension_value[2] + Before_Tension_value2[2]) / 3;//�ړ�����
				Before_Tension_value2[2] = Before_Tension_value[2];//�ړ�����
				Before_Tension_value[2] = Tension_value[2];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 2, Tension_value[2], TRUE);
				if (Safety_value[2] <= Tension_value[2]) {
					VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[2] - AD_TMP[2] >= 200 && Auto_check[2] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 2, &errorCode);
					Auto_check[2] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &Tension_value[3], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[3] = (Tension_value[3] + Before_Tension_value[3] + Before_Tension_value2[3]) / 3;//�ړ�����
				Before_Tension_value2[3] = Before_Tension_value[3];//�ړ�����
				Before_Tension_value[3] = Tension_value[3];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 3, Tension_value[3], TRUE);
				if (Safety_value[3] <= Tension_value[3]) {
					VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[3] - AD_TMP[3] >= 20 && Auto_check[3] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 3, 512, 0, 1, &errorCode);////////
					Auto_check[3] = 1;
					KillTimer(strat, 6);
				}
			}
			return 1;

		case 7://3:�d�w�Η������̎����ݒ�
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &Tension_value[3], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[3] = (Tension_value[3] + Before_Tension_value[3] + Before_Tension_value2[3]) / 3;//�ړ�����
				Before_Tension_value2[3] = Before_Tension_value[3];//�ړ�����
				Before_Tension_value[3] = Tension_value[3];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 3, Tension_value[3], TRUE);
				if (Safety_value[3] <= Tension_value[3]) {
					VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[3] - AD_TMP[3] >= 100 && Auto_check[3] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 3, &errorCode);
					Auto_check[3] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &Tension_value[2], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[2] = (Tension_value[2] + Before_Tension_value[2] + Before_Tension_value2[2]) / 3;//�ړ�����
				Before_Tension_value2[2] = Before_Tension_value[2];//�ړ�����
				Before_Tension_value[2] = Tension_value[2];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 2, Tension_value[2], TRUE);
				if (Safety_value[2] <= Tension_value[2]) {
					VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[2] - AD_TMP[2] >= 20 && Auto_check[2] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 2, 256, 0, 1, &errorCode);////////
					Auto_check[2] = 1;
					KillTimer(strat, 7);
				}
			}
			return 1;

		case 8://4:�d�w���Ȃ̎����ݒ�@
			if (Auto_setting == 0) {//�ݒ肪�������Ă�����
				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &Tension_value[4], &errorCode);//�����̓Z���T�̌��݂̒l�@VCS_GetAnalogInput:�A�i���O���͂���l��Ԃ�
				Tension_value[4] = (Tension_value[4] + Before_Tension_value[4] + Before_Tension_value2[4]) / 3;//�ړ����ρ@Tension_value�F�A�i���O�l���i�[����ϐ�
				Before_Tension_value2[4] = Before_Tension_value[4];//�ړ�����
				Before_Tension_value[4] = Tension_value[4];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 4, Tension_value[4], TRUE);
				if (Safety_value[4] <= Tension_value[4]) {
					VCS_MoveToPosition(keyHandle, NodeId + 4, 0, 1, 1, &errorCode);////////VCS_MoveToPosition �ڕW�ʒu�܂ňړ��@3�Ԗ�(0)���ڕW�ʒu
				}
				if (Tension_value[4] - AD_TMP[4] >= 150 && Auto_check[4] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 4, &errorCode);//�v���t�@�C���̌����ɂ���Ĉړ����~
					Auto_check[4] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &Tension_value[7], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[7] = (Tension_value[7] + Before_Tension_value[7] + Before_Tension_value2[7]) / 3;//�ړ�����
				Before_Tension_value2[7] = Before_Tension_value[7];//�ړ�����
				Before_Tension_value[7] = Tension_value[7];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 7, Tension_value[7], TRUE);
				if (Safety_value[7] <= Tension_value[7]) {
					VCS_MoveToPosition(keyHandle, NodeId + 7, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[7] - AD_TMP[7] >= 20 && Auto_check[7] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 7, 256, 0, 1, &errorCode);////////256�̈ʒu�܂ňړ�
					Auto_check[7] = 1;
					KillTimer(strat, 8);
				}
			}
			return 1;

		case 9://5:���w���Ȃ̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &Tension_value[5], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[5] = (Tension_value[5] + Before_Tension_value[5] + Before_Tension_value2[5]) / 3;//�ړ�����
				Before_Tension_value2[5] = Before_Tension_value[5];//�ړ�����
				Before_Tension_value[5] = Tension_value[5];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 5, Tension_value[5], TRUE);
				if (Safety_value[5] <= Tension_value[5]) {
					VCS_MoveToPosition(keyHandle, NodeId + 5, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[5] - AD_TMP[5] >= 400 && Auto_check[5] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 5, &errorCode);//�v���t�@�C���̌����ɂ���Ĉړ����~
					Auto_check[5] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &Tension_value[8], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[8] = (Tension_value[8] + Before_Tension_value[8] + Before_Tension_value2[8]) / 3;//�ړ�����
				Before_Tension_value2[8] = Before_Tension_value[8];//�ړ�����
				Before_Tension_value[8] = Tension_value[8];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 8, Tension_value[8], TRUE);
				if (Safety_value[8] <= Tension_value[8]) {
					VCS_MoveToPosition(keyHandle, NodeId + 8, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[8] - AD_TMP[8] >= 20 && Auto_check[8] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 8, 256, 0, 1, &errorCode);/////////256�̈ʒu�܂ňړ�
					Auto_check[8] = 1;
					KillTimer(strat, 9);
				}
			}
			return 1;

		case 10://6:3�w���Ȃ̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &Tension_value[6], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[6] = (Tension_value[6] + Before_Tension_value[6] + Before_Tension_value2[6]) / 3;//�ړ�����
				Before_Tension_value2[6] = Before_Tension_value[6];//�ړ�����
				Before_Tension_value[6] = Tension_value[6];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 6, Tension_value[6], TRUE);
				/*	if (Safety_value[6] <= Tension_value[6]) {
						VCS_MoveToPosition(keyHandle, NodeId + 6, 0, 1, 1, &errorCode);////////
					}*/
				if (Tension_value[6] - AD_TMP[6] >= 350 && Auto_check[6] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 6, &errorCode);
					Auto_check[6] = 1;
				}
			}
			/*		if (Auto_setting == 2) {
						VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &Tension_value[9], &errorCode);//�����̓Z���T�̌��݂̒l
						Tension_value[9] = (Tension_value[9] + Before_Tension_value[9] + Before_Tension_value2[9]) / 3;//�ړ�����
						Before_Tension_value2[9] = Before_Tension_value[9];//�ړ�����
						Before_Tension_value[9] = Tension_value[9];//�ړ�����
						SetDlgItemInt(strat, IDC_Start_AD1 + 9, Tension_value[9], TRUE);
						if (Safety_value[9] <= Tension_value[9]) {
							VCS_MoveToPosition(keyHandle, NodeId + 9, 0, 1, 1, &errorCode);////////
						}
						if (Tension_value[9] - AD_TMP[9] >= 100 && Auto_check[9] == 0) {
							VCS_MoveToPosition(keyHandle, NodeId + 9, 512, 0, 1, &errorCode);////////
							Auto_check[9] = 1;
							KillTimer(strat, 10);
						}
					}*/
			return 1;

		case 11://7:�d�w�L�W�̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 7, 1, &Tension_value[7], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[7] = (Tension_value[7] + Before_Tension_value[7] + Before_Tension_value2[7]) / 3;//�ړ�����
				Before_Tension_value2[7] = Before_Tension_value[7];//�ړ�����
				Before_Tension_value[7] = Tension_value[7];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 7, Tension_value[7], TRUE);
				if (Safety_value[7] <= Tension_value[7]) {
					VCS_MoveToPosition(keyHandle, NodeId + 7, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[7] - AD_TMP[7] >= 600 && Auto_check[7] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 7, &errorCode);
					Auto_check[7] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 4, 1, &Tension_value[4], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[4] = (Tension_value[4] + Before_Tension_value[4] + Before_Tension_value2[4]) / 3;//�ړ�����
				Before_Tension_value2[4] = Before_Tension_value[4];//�ړ�����
				Before_Tension_value[4] = Tension_value[4];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 4, Tension_value[4], TRUE);
				if (Safety_value[4] <= Tension_value[4]) {
					VCS_MoveToPosition(keyHandle, NodeId + 4, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[4] - AD_TMP[4] >= 20 && Auto_check[4] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 4, 768, 0, 1, &errorCode);////////
					Auto_check[4] = 1;
					KillTimer(strat, 11);
				}
			}
			return 1;

		case 12://8:���w�L�W�̎����ݒ�@
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 8, 1, &Tension_value[8], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[8] = (Tension_value[8] + Before_Tension_value[8] + Before_Tension_value2[8]) / 3;//�ړ�����
				Before_Tension_value2[8] = Before_Tension_value[8];//�ړ�����
				Before_Tension_value[8] = Tension_value[8];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 8, Tension_value[8], TRUE);
				if (hara == 0)
				{
					if (Safety_value[8] <= Tension_value[8]) {
						VCS_MoveToPosition(keyHandle, NodeId + 8, 0, 1, 1, &errorCode);
					}
					else if (Tension_value[8] - AD_TMP[8] >= 550 && Auto_check[8] == 0) {//250
						VCS_HaltPositionMovement(keyHandle, NodeId + 8, &errorCode);
						Auto_check[8] = 1;
					}
				}
				if (hara == 1) {//�w��
					if (Safety_value[8] <= Tension_value[8]) {
						VCS_MoveToPosition(keyHandle, NodeId + 8, 0, 1, 1, &errorCode);
					}
					else if (Tension_value[8] - AD_TMP[8] >= 300 && Auto_check[8] == 0) {//250
						VCS_HaltPositionMovement(keyHandle, NodeId + 8, &errorCode);
						Auto_check[8] = 1;
					}
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 5, 1, &Tension_value[5], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[5] = (Tension_value[5] + Before_Tension_value[5] + Before_Tension_value2[5]) / 3;//�ړ�����
				Before_Tension_value2[5] = Before_Tension_value[5];//�ړ�����
				Before_Tension_value[5] = Tension_value[5];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 5, Tension_value[5], TRUE);
				if (Safety_value[5] <= Tension_value[5]) {
					VCS_MoveToPosition(keyHandle, NodeId + 5, 0, 1, 1, &errorCode);
				}
				else if (Tension_value[5] - AD_TMP[5] >= 15 && Auto_check[5] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 5, 256, 0, 1, &errorCode);
					Auto_check[5] = 1;
					KillTimer(strat, 12);
				}
			}
			return 1;

		case 13://9:3�w�L�W�̎����ݒ�
			if (Auto_setting == 0) {
				VCS_GetAnalogInput(keyHandle, NodeId + 9, 1, &Tension_value[9], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[9] = (Tension_value[9] + Before_Tension_value[9] + Before_Tension_value2[9]) / 3;//�ړ�����
				Before_Tension_value2[9] = Before_Tension_value[9];//�ړ�����
				Before_Tension_value[9] = Tension_value[9];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 9, Tension_value[9], TRUE);
				if (Safety_value[9] <= Tension_value[9]) {
					VCS_MoveToPosition(keyHandle, NodeId + 9, 0, 1, 1, &errorCode);////////
				}//1500
				if (Tension_value[9] - AD_TMP[9] >= 2000 && Auto_check[9] == 0) {
					VCS_HaltPositionMovement(keyHandle, NodeId + 9, &errorCode);
					Auto_check[9] = 1;
				}
			}
			if (Auto_setting == 2) {
				VCS_GetAnalogInput(keyHandle, NodeId + 6, 1, &Tension_value[6], &errorCode);//�����̓Z���T�̌��݂̒l
				Tension_value[6] = (Tension_value[6] + Before_Tension_value[6] + Before_Tension_value2[6]) / 3;//�ړ�����
				Before_Tension_value2[6] = Before_Tension_value[6];//�ړ�����
				Before_Tension_value[6] = Tension_value[6];//�ړ�����
				SetDlgItemInt(strat, IDC_Start_AD1 + 6, Tension_value[6], TRUE);
				if (Safety_value[6] <= Tension_value[6]) {
					VCS_MoveToPosition(keyHandle, NodeId + 6, 0, 1, 1, &errorCode);////////
				}
				if (Tension_value[6] - AD_TMP[6] >= 15 && Auto_check[6] == 0) {
					VCS_MoveToPosition(keyHandle, NodeId + 6, 256, 0, 1, &errorCode);////////
					Auto_check[6] = 1;
					KillTimer(strat, 13);
				}
			}
			return 1;

			/*	case 14://2:�d�w�Η��̎����ݒ�@
					if (Auto_setting == 0) {
						VCS_GetAnalogInput(keyHandle, NodeId + 2, 1, &Tension_value[2], &errorCode);//�����̓Z���T�̌��݂̒l
						Tension_value[2] = (Tension_value[2] + Before_Tension_value[2] + Before_Tension_value2[2]) / 3;//�ړ�����
						Before_Tension_value2[2] = Before_Tension_value[2];//�ړ�����
						Before_Tension_value[2] = Tension_value[2];//�ړ�����
						SetDlgItemInt(strat, IDC_Start_AD1 + 2, Tension_value[2], TRUE);
						if (Safety_value[2] <= Tension_value[2]) {
							VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);////////
						}
						if (Tension_value[2] - AD_TMP[2] >= 250 && Auto_check[2] == 0) {
							VCS_HaltPositionMovement(keyHandle, NodeId + 2, &errorCode);
							Auto_check[2] = 1;
						}
					}
					if (Auto_setting == 2) {
						VCS_GetAnalogInput(keyHandle, NodeId + 3, 1, &Tension_value[3], &errorCode);//�����̓Z���T�̌��݂̒l
						Tension_value[3] = (Tension_value[3] + Before_Tension_value[3] + Before_Tension_value2[3]) / 3;//�ړ�����
						Before_Tension_value2[3] = Before_Tension_value[3];//�ړ�����
						Before_Tension_value[3] = Tension_value[3];//�ړ�����
						SetDlgItemInt(strat, IDC_Start_AD1 + 3, Tension_value[3], TRUE);
						if (Safety_value[3] <= Tension_value[3]) {
							VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);////////
						}
						if (Tension_value[3] - AD_TMP[3] >= 20 && Auto_check[3] == 0) {
							VCS_MoveToPosition(keyHandle, NodeId + 3, 256, 0, 1, &errorCode);////////
							Auto_check[3] = 1;
							KillTimer(strat, 14);
						}
					}
					return 1;*/

					//�w����̊m�F
		case 15://"�O�["�����ݒ�
			if (Auto_check[4] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;//�I�[�g�ݒ�̍ۂ̕ϐ��@1,�L�W�܂��͋��Ȃ̎�������
			}

			if (Auto_setting == 1) {//���Ȃ̏�Ԃ̂܂ܐݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;//�ݒ芮��
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 15);
			}
			return 1;

		case 16://"�p�["�����ݒ�
			if (Auto_check[7] == 1 && Auto_check[8] == 1 && Auto_check[9] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�L�W�̏�Ԃ̂܂ܐݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;//�ݒ芮��
				EnableWindow(GetDlgItem(strat, IDC_Start_Ext3), 1);
				SetDlgItemText(strat, IDC_Ext_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 16);
			}
			return 1;

		case 17://"���w�L�W"���Ȏ����ݒ�
			if (Auto_check[4] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 17);
			}
			return 1;

		case 18://"���w�L�W"�L�W�����ݒ�
			if (Auto_check[4] && Auto_check[6] == 1 && Auto_check[8] && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Ext3), 1);
				SetDlgItemText(strat, IDC_Ext_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 18);
			}
			return 1;

		case 19://�w��܂ݎ����ݒ�
			if (Auto_check[2] == 1 && Auto_check[4] == 1 && Auto_check[5] == 1 /*&& Auto_check[9] == 1*/ && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 19);
			}
			return 1;

		case 20://�܂݉��������ݒ�
			if (Auto_check[3] == 1 && Auto_check[7] == 1 && Auto_check[8] == 1 /*&& Auto_check[9] == 1*/ && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Ext_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 20);
			}
			return 1;

		case 21://�d�w���]�����ݒ�
			if (Auto_check[0] == 1 /*&& Auto_check[2] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_check[7] == 1*/ && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//���Ȃ̏�Ԃ̂܂ܐݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 21);
			}
			return 1;

		case 22://�d�w�O�]�����ݒ�
			if (Auto_check[1] == 1 && Auto_check[3] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_check[7] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//���Ȃ̏�Ԃ̂܂ܐݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Ext3), 1);
				SetDlgItemText(strat, IDC_Ext_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 22);
			}
			return 1;

		case 23: //��w��]a//
			if (Auto_check[0] == 1 && /*Auto_check[7] == 1*/  Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}


			if (Auto_setting == 1) {
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Rotation3), 1);
				SetDlgItemText(strat, IDC_Rotation_Edit1, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 23);

			}
			return 1;

		case 24://��w��]b �Η������@�O�]//
			if (Auto_check[1] == 1 && Auto_check[3] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ��� 1356
				Auto_setting = 1;
			}


			if (Auto_setting == 1) {
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Rotation3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 24);

			}
			return 1;

		case 25://���e����//
			if (Auto_check[0] == 1 && Auto_check[2] == 1 && Auto_check[5] == 1 && Auto_check[6] == 1 && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ��� 1356
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Rotation3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 25);

			}
			return 1;

		case 26://��w�L�W//
			if (Auto_check[7] == 1 /*&& Auto_check[5] == 1 /*&& Auto_check[6] == 1 */ && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ��� 1356
				Auto_setting = 1;
			}
			if (Auto_setting == 1) {

				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}

			}

			Auto_setting = 0;
			EnableWindow(GetDlgItem(start, IDC_Start_Rotation3), 1);
			SetDlgItemText(strat, IDC_Rotation_Edit1, "�v�ۑ�");
			SetTimer(start, 1, 100, NULL);
			KillTimer(strat, 26);


			return 1;

		case 27://�w���܂�
			if (Auto_check[2] == 1 && Auto_check[5] == 1 /*&& Auto_check[9] == 1*/ && Auto_check[8] && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
			}
			Auto_setting = 0;
			EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
			SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
			SetTimer(strat, 1, 100, NULL);
			KillTimer(strat, 27);


		case 28://�d�w����
			if (/*Auto_check[2] == 1 && Auto_check[5] == 1 && Auto_check[9] == 1 &&*/ Auto_check[4] && Auto_setting == 0) {//�K�v�Ȏw�̐�����Auto_check�������ɂ���
				Auto_setting = 1;
			}

			if (Auto_setting == 1) {//�ݒ�I��
				for (i = 0; i <= 9; i++) {
					Auto_check[i] = 0;
				}
				Auto_setting = 0;
				EnableWindow(GetDlgItem(strat, IDC_Start_Fle3), 1);
				SetDlgItemText(strat, IDC_Fle_Edit, "�v�ۑ�");
				SetTimer(strat, 1, 100, NULL);
				KillTimer(strat, 28);
			}



		}
		// �����ŏ������Ȃ��R���g���[�����I�����ꂽ�ꍇ��Windows�ɔC���邽�߂�0��Ԃ��B 
		return 0;

		//����������
	case WM_INITDIALOG:
		VCS_SetProtocolStackSettings(keyHandle, 1000000, 500, &errorCode);
		cserial = new CSerial; 						// Cserial�N���X���擾
		cserial->MakeBuffer(2, 27); 					// ���M�p�f�[�^��2byte�A��M�p�f�[�^��9byte�p�ӂ���B
		if (cserial->GetComNum(NULL) == 0) {
			MessageBox(strat, "�ڑ����m�F�ł��܂���", "error", MB_OK);
		}
		cserial->SetFindSerialPortName(0);		// COM�̐ݒ�
		//cserial->SetSerialPortName("COM3");			// COM�̐ݒ�
		cserial->OpenSerialPort(); 					// �V���A���|�[�g���I�[�v������B
		cserial->SetSerialPort(19200, 1024, 1024);	// �{�[���C�g�̐ݒ� 19200bps
		cserial->SerialPortBufferClear(); 			// �V���A���|�[�g�̑���MFIFO���������N���A����B
		SetDlgItemText(strat, IDC_Fle_Edit, "���ݒ�");
		SetDlgItemText(strat, IDC_Ext_Edit, "���ݒ�");
		SetDlgItemText(strat, IDC_Rotation_Edit1, "���ݒ�");
		return 1;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE:
		DestroyWindow(strat);
		return 1;

	case WM_DESTROY:		//�I��(�E�B���h�E��������)���� 
		PostQuitMessage(0);		// ���b�Z�[�W���[�v���甲���o���B 
		return 1;

	}
	return 0;
}

BOOL CALLBACK DialogSIMPLEREHA(HWND hwndSR, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD ID_SR;

	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Simple_Minus1:
			R_Target--;
			if (R_Target <= 0) { R_Target = 1; }
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Simple_Minus10:
			R_Target -= 10;
			if (R_Target <= 0) { R_Target = 1; }
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Simple_Plus1:
			R_Target++;
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Simple_Plus10:
			R_Target += 10;
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT1, R_Target, TRUE);
			return 1;

		case IDC_Simple_Reset:
			R_Num = 0;
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
			return 1;

		case IDC_Simple_Time:
			S = timeGetTime();
			E = timeGetTime();
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT4, (E - S) / 1000, TRUE);
			return 1;

		case IDC_RADIORIHAG:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 0;
			re = 0;

			return 1;

		case IDC_RADIORIHAP:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 1;
			re = 1;
			return 1;

		case IDC_RADIORIHAZ:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 2;
			re = 2;
			return 1;

		case IDC_RADIORIHAR:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 3;
			re = 3;
			return 1;

		case IDC_RIHARATE:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 4;
			re = 4;
			return 1;

		case IDC_RIHAHARA:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_UNCHECKED, 0);
			rf = 5;
			re = 1;
			return 1;

		case IDC_RIHAKU:
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAG), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAP), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAZ), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RADIORIHAR), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHARATE), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAHARA), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndSR, IDC_RIHAKU), BM_SETCHECK, BST_CHECKED, 0);
			rf = 6;
			re = 6;
			return 1;




		case IDC_REPTITION:

			hwndRS = CreateDialog(reptition, "REPTITION", NULL, (DLGPROC)DialogRS);//�_�C�A���O�{�b�N�X���\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndRS == NULL)
			{
				MessageBox(NULL, "�_�C�A���O�{�b�N�X��ID���Ԉ���Ă��܂���?", "�I��", MB_OK);
				return 0;
			}

			//(2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndRS, SW_SHOW);
			UpdateWindow(hwndRS);

			return 1;




		case IDC_Simple_Normal_Fle_Reha:
			SetTimer(hwndSR, 3, 20, NULL);
			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
			//�����ʒu�Ɏw�𓮂���
			Reach_check[10] = 0;
			Reach_check[11] = 0;
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					Reach_check[10]++;
					Reach_check[i] = 0;
					VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
				}
			}
			Reha_end = 0;
			Reha_close = 1;
			SetTimer(hwndSR, 4, 20, NULL);
			return 1;

		case IDC_Simple_Sensor_Fle_Reha:
			if (Used_Sensor_amount != 0) {
				if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_Log)) == BST_CHECKED) {
					now = time(NULL);
					date = localtime(&now);
					strftime(File, sizeof(File), "../rehabili_data/%Y%m%d%H%M.csv", date);
					fp = fopen(File, "w");
					fileopen = 1;
					fprintf(fp, "����,MSP,�ؓd1,�ؓd2,�ؓd3,�ؓd4,���],�O�],��w����,���w����,������,���w�L�W,���L�W,th1,th2,th3,th4,th5,th6,th7,th8,th9,th10,th11,th12,���],�O�],�Η�,�Η�����,�d�w����,���w����,3�w����,�d�w�L�W,���w�L�W,3�w�L�W,�ڕW��,���s��\n");
					SetTimer(hwndSR, 2, 50, NULL);
				}
				SetTimer(hwndSR, 3, 20, NULL);
				S = timeGetTime();
				SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "���쒆");
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
				//�����ʒu�Ɏw�𓮂���
				Reach_check[10] = 0;
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
						Reach_check[10]++;
						Reach_check[i] = 0;
						Halt_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
					}
				}
				Fle_Sensor_mode = 0;
				Ext_Sensor_mode = 0;
				Before_Sensor_Reha_Flag = 0;
				Reha_end = 0;
				Reha_close = 1;
				SetTimer(hwndSR, 6, 20, NULL);
			}
			return 1;

		case IDC_Simple_Sensor_Fle_Reha_Once:
			if (Used_Sensor_amount != 0) {
				if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_Log)) == BST_CHECKED) {
					now = time(NULL);
					date = localtime(&now);
					strftime(File, sizeof(File), "../rehabili_data/%Y%m%d%H%M.csv", date);
					fp = fopen(File, "w");
					fileopen = 1;
					fprintf(fp, "����,MSP,�ؓd1,�ؓd2,�ؓd3,�ؓd4,���],�O�],��w����,���w����,������,���w�L�W,���L�W,th1,th2,th3,th4,th5,th6,th7,th8,th9,th10,th11,th12,���],�O�],�Η�,�Η�����,�d�w����,���w����,3�w����,�d�w�L�W,���w�L�W,3�w�L�W,�ڕW��,���s��\n");
					SetTimer(hwndSR, 2, 50, NULL);
				}
				SetTimer(hwndSR, 3, 20, NULL);
				S = timeGetTime();
				SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
				//�����ʒu�Ɏw�𓮂���
				Reach_check[10] = 0;
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
						Reach_check[10]++;
						Reach_check[i] = 0;
						Halt_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
					}
				}
				Reha_end = 0;
				Reha_close = 1;
				SetTimer(hwndSR, 8, 20, NULL);
			}
			return 1;

		case IDC_Simple_Normal_Ext_Reha:
			SetTimer(hwndSR, 3, 20, NULL);
			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
			//�����ʒu�Ɏw�𓮂���
			Reach_check[10] = 0;
			Reach_check[11] = 0;
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					Reach_check[10]++;
					Reach_check[i] = 0;
					VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
				}
			}
			Reha_end = 0;
			Reha_close = 1;
			SetTimer(hwndSR, 5, 20, NULL);
			return 1;

		case IDC_Simple_Normal_Rot_Reha:
			SetTimer(hwndSR, 3, 20, NULL);
			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
			//�����ʒu�Ɏw�𓮂���
			Reach_check[10] = 0;
			Reach_check[11] = 0;
			for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
					Reach_check[10]++;
					Reach_check[i] = 0;
					VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[1][i], 1, 0, &errorCode);
				}
			}
			Reha_end = 0;
			Reha_close = 1;
			SetTimer(hwndSR, 10, 20, NULL);

			return 1;

		case IDC_test: //���炩���[�u�@�e�X�g
			SetTimer(hwndSR, 3, 20, NULL);
			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
			//�����ʒu�Ɏw�𓮂���
			Reach_check[10] = 0;
			Reach_check[11] = 0;
			/*for (i = 0; i < 10; i++) {
				if (Connection_Check[i] == 1) {
					VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
						Reach_check[10]++;
						Reach_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[1][i], 1, 0, &errorCode);
				}
			}*/

			test_flag = 0;
			Reha_end = 0;
			Reha_close = 1;
			SetTimer(hwndSR, 11, 20, NULL);

			return 1;


		case IDC_Simple_Sensor_Ext_Reha:
			if (Used_Sensor_amount != 0) {
				if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_Log)) == BST_CHECKED) {
					now = time(NULL);
					date = localtime(&now);
					strftime(File, sizeof(File), "../rehabili_data/%Y%m%d%H%M.csv", date);
					fp = fopen(File, "w");
					fileopen = 1;
					fprintf(fp, "����,MSP,�ؓd1,�ؓd2,�ؓd3,�ؓd4,���],�O�],��w����,���w����,������,���w�L�W,���L�W,th1,th2,th3,th4,th5,th6,th7,th8,th9,th10,th11,th12,���],�O�],�Η�,�Η�����,�d�w����,���w����,3�w����,�d�w�L�W,���w�L�W,3�w�L�W,�ڕW��,���s��\n");
					SetTimer(hwndSR, 2, 50, NULL);
				}
				SetTimer(hwndSR, 3, 20, NULL);
				S = timeGetTime();
				SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "���쒆");
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
				//�����ʒu�Ɏw�𓮂���
				Reach_check[10] = 0;
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
						Reach_check[10]++;
						Reach_check[i] = 0;
						Halt_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
					}
				}
				Fle_Sensor_mode = 0;
				Ext_Sensor_mode = 0;
				Before_Sensor_Reha_Flag = 0;
				Reha_end = 0;
				Reha_close = 1;
				SetTimer(hwndSR, 7, 20, NULL);
			}
			return 1;

		case IDC_Simple_Sensor_Ext_Reha_Once:
			if (Used_Sensor_amount != 0) {
				if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_Log)) == BST_CHECKED) {
					now = time(NULL);
					date = localtime(&now);
					strftime(File, sizeof(File), "../rehabili_data/%Y%m%d%H%M.csv", date);
					fp = fopen(File, "w");
					fileopen = 1;
					fprintf(fp, "����,MSP,�ؓd1,�ؓd2,�ؓd3,�ؓd4,���],�O�],��w����,���w����,������,���w�L�W,���L�W,th1,th2,th3,th4,th5,th6,th7,th8,th9,th10,th11,th12,���],�O�],�Η�,�Η�����,�d�w����,���w����,3�w����,�d�w�L�W,���w�L�W,3�w�L�W,�ڕW��,���s��\n");
					SetTimer(hwndSR, 2, 50, NULL);
				}
				SetTimer(hwndSR, 3, 20, NULL);
				S = timeGetTime();
				SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
				//�����ʒu�Ɏw�𓮂���
				Reach_check[10] = 0;
				Reach_check[11] = 0;
				for (i = 0; i < 10; i++) {
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 1000, 1000, 1000, &errorCode);
						Reach_check[10]++;
						Reach_check[i] = 0;
						Halt_check[i] = 0;
						VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
					}
				}
				Reha_end = 0;
				Reha_close = 1;
				SetTimer(hwndSR, 9, 20, NULL);
			}
			return 1;

		case IDC_REPSTART:
			SetTimer(hwndSR, 3, 20, NULL);
			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);
			SetTimer(hwndSR, 14, 20, NULL);

			return 1;

		case IDC_REPSIDE:
			SetTimer(hwndSR, 3, 20, NULL);

			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);

			test_flag = 0;
			Reha_end = 0;
			Reha_close = 1;

			SetTimer(hwndSR, 12, 20, NULL);
			return 1;

		case IDC_REPLEN:
			SetTimer(hwndSR, 3, 20, NULL);

			S = timeGetTime();
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 0);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 0);

			SetTimer(hwndSR, 13, 20, NULL);

			return 1;


		case IDC_Simple_Stop:
			Reha_end = 1;
			if (fileopen == 1) {
				fileopen = 0;
				fclose(fp);
			}
			KillTimer(hwndSR, 2);
			KillTimer(hwndSR, 3);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
			EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
			return 1;

		case IDC_Reha_CHECK1:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK1)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[0] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[0] = 0;
				if (Dlg_Sensor[0] >= Dlg_Th[0] && Exceed_falg[0] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[0] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK2:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK2)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[1] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[1] = 0;
				if (Dlg_Sensor[1] >= Dlg_Th[1] && Exceed_falg[1] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[1] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK3:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK3)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[2] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[2] = 0;
				if (Dlg_Sensor[2] >= Dlg_Th[2] && Exceed_falg[2] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[2] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK4:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK4)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[3] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[3] = 0;
				if (Dlg_Sensor[3] >= Dlg_Th[3] && Exceed_falg[3] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[3] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK5:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK5)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[4] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[4] = 0;
				if (Dlg_Sensor[4] >= Dlg_Th[4] && Exceed_falg[4] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[4] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK6:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK6)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[5] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[5] = 0;
				if (Dlg_Sensor[5] >= Dlg_Th[5] && Exceed_falg[5] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[5] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK7:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK7)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[6] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[6] = 0;
				if (Dlg_Sensor[6] >= Dlg_Th[6] && Exceed_falg[6] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[6] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK8:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK8)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[7] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[7] = 0;
				if (Dlg_Sensor[7] >= Dlg_Th[7] && Exceed_falg[7] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[7] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK9:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK9)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[8] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[8] = 0;
				if (Dlg_Sensor[8] >= Dlg_Th[8] && Exceed_falg[8] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[8] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK10:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK10)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[9] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[9] = 0;
				if (Dlg_Sensor[9] >= Dlg_Th[9] && Exceed_falg[9] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[9] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK11:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK11)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[10] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[10] = 0;
				if (Dlg_Sensor[10] >= Dlg_Th[10] && Exceed_falg[10] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[10] = 0;
				}
			}
			return 1;

		case IDC_Reha_CHECK12:
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK12)) == BST_CHECKED) {
				Used_Sensor_amount++;
				Used_Sensor[11] = 1;
			}
			else {
				Used_Sensor_amount--;
				Used_Sensor[11] = 0;
				if (Dlg_Sensor[11] >= Dlg_Th[11] && Exceed_falg[11] == 1) {
					Exceed_Sensor_amount--;
					Exceed_falg[11] = 0;
				}
			}
			return 1;

		}
		return 0;

		//����������
	case WM_INITDIALOG:
		InitCommonControls();
		SetDlgItemInt(hwndSR, IDC_Simple_EDIT1, 50, TRUE);
		SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
		SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "��~��");
		SetDlgItemInt(hwndSR, IDC_Simple_EDIT4, 0, TRUE);
		SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
		SendMessage(GetDlgItem(hwndSR, IDC_Reha_Log), BM_SETCHECK, BST_CHECKED, 0);
		Used_Sensor_amount = 0;
		for (i = 0; i < 12; i++) {
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS1 + i), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS1 + i), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS1 + i), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i), TBM_SETRANGE, TRUE, MAKELPARAM(1, 4096)); //�X���C�_�[�R���g���[���̃����W���w��
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i), TBM_SETTICFREQ, 1, 0);   // �X���C�_�[�R���g���[���̖ڐ���̑���
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i), TBM_SETPOS, TRUE, 3000);  // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
			SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i), TBM_SETPAGESIZE, 0, 50); // �X���C�_�[�R���g���[���̃N���b�N���̈ړ���
			SetDlgItemInt(hwndSR, IDC_Reha_Sensor1 + i, 0, 1);
			SetDlgItemInt(hwndSR, IDC_Reha_th1 + i, 3000, 1);
			if (Used_Sensor[i] == 1) {
				SendMessage(GetDlgItem(hwndSR, IDC_Reha_CHECK1 + i), BM_SETCHECK, BST_CHECKED, 1);
				Used_Sensor_amount++;
			}
		}
		SetTimer(hwndSR, 1, 20, NULL);//�Z���T�[�̒l�擾
		return 1;

	case WM_HSCROLL://�X���C�_�[�R���g���[���𓮂������Ƃ�
		for (i = 0; i < 12; i++) {
			if (GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i) == (HWND)lParam)
			{
				Slider_Value = SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER1 + i), TBM_GETPOS, NULL, NULL); // �X���C�_�[�R���g���[���̌��݂̒l�̎擾
				SetDlgItemInt(hwndSR, IDC_Reha_th1 + i, Slider_Value, 1);
			}
		}
		break;

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
			/*case 1: //�V���A���ʐM�̒l�̑���M�Ƃ�
				cserial->m_senddata[0] = 1;		 	// ���얽�߂𑗂�
				cserial->m_senddata[1] = 0;			// �g���ĂȂ�
				cserial->SendSerialData(2);						// �p�\�R������dsPIC��1byte�̃f�[�^�𑗐M
				cserial->ReceiveSerialData(27);				// �p�\�R���ɗ��Ă���V���A���f�[�^��9byte��M

				//�ؓd1
				Sensor_data[2] = cserial->m_receivedata[5] << 8;
				Sensor_data[2] |= cserial->m_receivedata[4];
				//�ؓd2
				Sensor_data[3] = cserial->m_receivedata[7] << 8;
				Sensor_data[3] |= cserial->m_receivedata[6];
				//�ؓd3
				Sensor_data[4] = cserial->m_receivedata[9] << 8;
				Sensor_data[4] |= cserial->m_receivedata[8];
				//�ؓd4
				Sensor_data[5] = cserial->m_receivedata[11] << 8;
				Sensor_data[5] |= cserial->m_receivedata[10];
				//���]
				Sensor_data[6] = cserial->m_receivedata[13] << 8;
				Sensor_data[6] |= cserial->m_receivedata[12];
				//�O�]
				Sensor_data[7] = cserial->m_receivedata[15] << 8;
				Sensor_data[7] |= cserial->m_receivedata[14];
				//��w����
				Sensor_data[8] = cserial->m_receivedata[17] << 8;
				Sensor_data[8] |= cserial->m_receivedata[16];
				//���w����
				Sensor_data[9] = cserial->m_receivedata[19] << 8;
				Sensor_data[9] |= cserial->m_receivedata[18];
				//�wor���w����
				Sensor_data[10] = cserial->m_receivedata[21] << 8;
				Sensor_data[10] |= cserial->m_receivedata[20];
				//���w�L�W
				Sensor_data[11] = cserial->m_receivedata[23] << 8;
				Sensor_data[11] |= cserial->m_receivedata[22];
				//�wor���w�L�W
				Sensor_data[12] = cserial->m_receivedata[25] << 8;
				Sensor_data[12] |= cserial->m_receivedata[24];
				//MSP
				Sensor_data[13] = cserial->m_receivedata[26];    //26

				//�Z���T�̒l�\��
				//�؎��k
				if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK1)) == BST_CHECKED) {
					SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS1), PBM_SETPOS, Sensor_data[13], 0);
					SetDlgItemInt(hwndSR, IDC_Reha_Sensor1, Sensor_data[13], 1);
				}
				else {
					SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS1), PBM_SETPOS, 0, 0);
					SetDlgItemInt(hwndSR, IDC_Reha_Sensor1, 0, 1);
				}

				for (i = 0; i <= 10; i++) {
					if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK2 + i)) == BST_CHECKED) {
						SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS2 + i), PBM_SETPOS, Sensor_data[2 + i], 0);
						SetDlgItemInt(hwndSR, IDC_Reha_Sensor2 + i, Sensor_data[2 + i], 1);
					}
					else {
						SendMessage(GetDlgItem(hwndSR, IDC_Reha_PROGRESS2 + i), PBM_SETPOS, 0, 0);
						SetDlgItemInt(hwndSR, IDC_Reha_Sensor2 + i, 0, 1);
					}
				}

				//�ڑ����Ă���Z���T�[��臒l�������Z���T�[�̐��̕\��
				SetDlgItemInt(hwndSR, IDC_Exceed_Amount, Exceed_Sensor_amount, 1);
				SetDlgItemInt(hwndSR, IDC_Used_Amount, Used_Sensor_amount, 1);

				//�G���R�[�_�[�̒l�̎擾
				for (i = 0; i < 10; i++) {
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						VCS_GetPositionIs(keyHandle, NodeId + i, &Reha_Motor[i], &errorCode);
					}
				}

				//臒l�𒴂����Z���T�[�̐��𐔂���//
				for (i = 0; i < 12; i++) {
					if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK1 + i)) == BST_CHECKED) {
						Dlg_Sensor[i] = GetDlgItemInt(hwndSR, IDC_Reha_Sensor1 + i, NULL, TRUE);
						Dlg_Th[i] = GetDlgItemInt(hwndSR, IDC_Reha_th1 + i, NULL, TRUE);
						if (Dlg_Sensor[i] >= Dlg_Th[i] && Exceed_falg[i] == 0) {
							Exceed_Sensor_amount++;
							Exceed_falg[i] = 1;
						}
						else if (Dlg_Sensor[i] < Dlg_Th[i] && Exceed_falg[i] == 1) {
							Exceed_Sensor_amount--;
							Exceed_falg[i] = 0;
						}
					}
				}

				//�Z���T�[���ȌP����臒l�̔���Ƃ��̏���
				if (Fle_Sensor_mode == 1) {
					//�Z���T�[��臒l�����Ă���
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
							if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
								VCS_GetPositionIs(keyHandle, NodeId + i, &Reha_Motor[i], &errorCode);
								if (Reach_check[i] == 0 && Reha_Motor[i] <= Flexion_Value[rf][i]) {
									Reach_check[i] = 1;
									Reach_check[11]++;
								}
							}
						}
					}
					if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {//臒l�ȉ�
						Before_Sensor_Reha_Flag = 0;
						SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "��~��");
						for (i = 0; i < 10; i++) {
							if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
								if (Halt_check[i] == 0) {
									VCS_HaltPositionMovement(keyHandle, NodeId + i, &errorCode);
									Halt_check[i] = 1;
								}
							}
						}
					}
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0 && Before_Sensor_Reha_Flag == 0) {
						Before_Sensor_Reha_Flag = 1;
						SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "���쒆");
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Halt_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
							}
						}
					}
				}
				//�Z���T�[�L�W�P����臒l�̔���Ƃ��̏���
				if (Ext_Sensor_mode == 1) {
					//�Z���T�[��臒l�����Ă���
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
							if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
								VCS_GetPositionIs(keyHandle, NodeId + i, &Reha_Motor[i], &errorCode);
								if (Reach_check[i] == 0 && Reha_Motor[i] <= Extension_Value[re][i]) {
									Reach_check[i] = 1;
									Reach_check[11]++;
								}
							}
						}
					}
					if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {//臒l�ȉ�
						Before_Sensor_Reha_Flag = 0;
						SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "��~��");
						for (i = 0; i < 10; i++) {
							if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
								if (Halt_check[i] == 0) {
									VCS_HaltPositionMovement(keyHandle, NodeId + i, &errorCode);
									Halt_check[i] = 1;
								}
							}
						}
					}
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0 && Before_Sensor_Reha_Flag == 0) {
						Before_Sensor_Reha_Flag = 1;
						SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "���쒆");
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Halt_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
							}
						}
					}
				}
				return 1;*/

		case 2://���n�r�����ԂƃZ���T�̒l�ƃZ���T��臒l�ƃG���R�[�_�[�̒l��ۑ�//
			if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_Log)) == BST_CHECKED) {
				fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", E - S, Dlg_Sensor[0], Dlg_Sensor[1], Dlg_Sensor[2], Dlg_Sensor[3], Dlg_Sensor[4], Dlg_Sensor[5], Dlg_Sensor[6], Dlg_Sensor[7], Dlg_Sensor[8], Dlg_Sensor[9], Dlg_Sensor[10], Dlg_Sensor[11], Dlg_Th[0], Dlg_Th[1], Dlg_Th[2], Dlg_Th[3], Dlg_Th[4], Dlg_Th[5], Dlg_Th[6], Dlg_Th[7], Dlg_Th[8], Dlg_Th[9], Dlg_Th[10], Dlg_Th[11], Reha_Motor[0], Reha_Motor[1], Reha_Motor[2], Reha_Motor[3], Reha_Motor[4], Reha_Motor[5], Reha_Motor[6], Reha_Motor[7], Reha_Motor[8], Reha_Motor[9], R_Target, R_Num);
			}
			return 1;

		case 3://�o�ߎ��Ԃ̑���
			E = timeGetTime();
			SetDlgItemInt(hwndSR, IDC_Simple_EDIT4, (E - S) / 1000, TRUE);
			return 1;

		case 4://���ȑ����^��

			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {

						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Extension_Value[re][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
						}
					}
					Reha_step = 1;
				}
			}
			if (Reha_step == 1) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//���ȓ����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
						}
					}
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					R_Num++;
					SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
					if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
							}
						}
						Reha_step = 1;
					}
				}
			}

			//���n�r���I���̏���
			if (R_Target <= R_Num || Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				KillTimer(hwndSR, 4);
			}
			return 1;

		case 5://�L�W�����^��
			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[re][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
						}
					}
					Reha_step = 1;
				}
			}
			if (Reha_step == 1) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//���ȓ����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
						}
					}
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					R_Num++;
					SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
					if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
							}
						}
						Reha_step = 1;
					}
				}
			}

			if (R_Target <= R_Num || Reha_end == 1) {//���n�r���I���̏���
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				KillTimer(hwndSR, 5);
			}
			return 1;

		case 6://�Z���T�[���ȉ^��
			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Extension_Value[re][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
						}
					}
					Fle_Sensor_mode = 1;
					Reha_step = 1;
				}
			}
			if (Reha_step == 1) {
				//���ȓ����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
						}
					}
					Fle_Sensor_mode = 0;
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					R_Num++;
					SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
					if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
							}
						}
						Fle_Sensor_mode = 1;
						Reha_step = 1;
					}
				}
			}
			if (R_Target <= R_Num || Reha_end == 1) {//���n�r���I���̏���
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				Fle_Sensor_mode = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "��~��");
				KillTimer(hwndSR, 6);
			}
			return 1;

		case 7://�Z���T�[�L�W�^��
			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[re][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
						}
					}
					Ext_Sensor_mode = 1;
					Reha_step = 1;
				}
			}
			if (Reha_step == 1) {
				//���ȓ����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
						}
					}
					Ext_Sensor_mode = 0;
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					R_Num++;
					SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
					if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
							}
						}
						Ext_Sensor_mode = 1;
						Reha_step = 1;
					}
				}
			}
			if (R_Target <= R_Num || Reha_end == 1) {//���n�r���I���̏���
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				Ext_Sensor_mode = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				SetDlgItemText(hwndSR, IDC_Simple_EDIT3, "��~��");
				KillTimer(hwndSR, 7);
			}
			return 1;

		case 8://�Z���T�[���ȌP��(1�x)
			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {
								Calcu_value[i] = ((-15 * Extension_Value[re][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
								if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
								VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
							}
						}
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								Halt_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
							}
						}
						Reha_step = 1;
					}
					else {
						if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {
							SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
						}
					}
				}
			}
			if (Reha_step == 1) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//�L�W�����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
						}

					}
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i <= 6; i++) {
						if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK6 + i)) == BST_CHECKED) {
							New_Threshold = GetDlgItemInt(hwndSR, IDC_Reha_Sensor6 + i, NULL, TRUE);//�Z���T�̒l�̃_�C�A���O�擾
							SetDlgItemInt(hwndSR, IDC_Reha_th6 + i, New_Threshold + 50, TRUE);//臒l�̃_�C�A���O�ɕ\��
							SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER6 + i), TBM_SETPOS, TRUE, New_Threshold + 50);//臒l�̃X���C�h�o�[�ړ� 
						}
					}
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
						R_Num++;
						SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
						if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
							Reach_check[11] = 0;
							for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
								if (Connection_Check[i] == 1) {
									Reach_check[i] = 0;
									VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
								}
							}
							Reha_step = 1;
						}
					}
				}
				else {
					if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
						for (i = 0; i <= 6; i++) {
							if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK6 + i)) == BST_CHECKED) {
								New_Threshold = GetDlgItemInt(hwndSR, IDC_Reha_Sensor6 + i, NULL, TRUE);//�Z���T�̒l�̃_�C�A���O�擾
								SetDlgItemInt(hwndSR, IDC_Reha_th6 + i, New_Threshold + 50, TRUE);//臒l�̃_�C�A���O�ɕ\��
								SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER6 + i), TBM_SETPOS, TRUE, New_Threshold + 50);//臒l�̃X���C�h�o�[�ړ� 
							}
						}
					}
				}
			}

			//���n�r���I���̏���
			if (R_Target <= R_Num || Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
				KillTimer(hwndSR, 8);
			}
			return 1;

		case 9://�Z���T�[�L�W�P��(1�x)
			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {
								Calcu_value[i] = ((-15 * Flexion_Value[rf][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[re][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
								if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
								VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
							}
						}
						Reach_check[11] = 0;
						for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
							if (Connection_Check[i] == 1) {
								Reach_check[i] = 0;
								Halt_check[i] = 0;
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
							}
						}
						Reha_step = 1;
					}
				}
				else {
					if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
					}
				}
			}
			if (Reha_step == 1) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//���ȓ����
				if (Reha_end == 0 && Reach_check[10] == Reach_check[11]) {
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[rf][i], 1, 0, &errorCode);
						}
					}
					Reha_step = 2;
				}
			}
			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�S�ĐL�W����܂őҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//����1�����I�����Ƃ̏���
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
					for (i = 0; i <= 6; i++) {
						if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK6 + i)) == BST_CHECKED) {
							New_Threshold = GetDlgItemInt(hwndSR, IDC_Reha_Sensor6 + i, NULL, TRUE);//�Z���T�̒l�̃_�C�A���O�擾
							SetDlgItemInt(hwndSR, IDC_Reha_th6 + i, New_Threshold + 50, TRUE);//臒l�̃_�C�A���O�ɕ\��
							SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER6 + i), TBM_SETPOS, TRUE, New_Threshold + 50);//臒l�̃X���C�h�o�[�ړ� 
						}
					}
					if (Exceed_Sensor_amount == Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "���쒆");
						R_Num++;
						SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
						if (R_Target > R_Num) {//�ڕW�l�����Ȃ�
							Reach_check[11] = 0;
							for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
								if (Connection_Check[i] == 1) {
									Reach_check[i] = 0;
									VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[re][i], 1, 0, &errorCode);
								}
							}
							Reha_step = 1;
						}
					}
				}
				else {
					if (Exceed_Sensor_amount < Used_Sensor_amount && Used_Sensor_amount != 0) {
						SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
						for (i = 0; i <= 6; i++) {
							if (Button_GetCheck(GetDlgItem(hwndSR, IDC_Reha_CHECK6 + i)) == BST_CHECKED) {
								New_Threshold = GetDlgItemInt(hwndSR, IDC_Reha_Sensor6 + i, NULL, TRUE);//�Z���T�̒l�̃_�C�A���O�擾
								SetDlgItemInt(hwndSR, IDC_Reha_th6 + i, New_Threshold + 50, TRUE);//臒l�̃_�C�A���O�ɕ\��
								SendMessage(GetDlgItem(hwndSR, IDC_Reha_SLIDER6 + i), TBM_SETPOS, TRUE, New_Threshold + 50);//臒l�̃X���C�h�o�[�ړ� 
							}
						}
					}
				}
			}

			if (R_Target <= R_Num || Reha_end == 1) {//���n�r���I���̏���
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
				KillTimer(hwndSR, 9);
			}
			return 1;


		case 10://��w��]�����^��

			if (Reha_step == 0) {
				for (i = 0; i < 10; i++) {//�����ʒu�܂őҋ@
					if (Connection_Check[i] == 1) {

						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//�L�W����Η�
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Rotation_Value[0][i]) / (sec * sec * 32)) - ((-15 * Rotation_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[1][i], 1, 1, &errorCode);
						}
					}
					Reha_step = 1;
				}
			}
			if (Reha_step == 1) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				//b-c
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//�Η��������
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Rotation_Value[1][i]) / (sec * sec * 32)) - ((-15 * Rotation_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[2][i], 1, 1, &errorCode);
						}
					}
					Reha_step = 2;
				}

			}

			if (Reha_step == 2) {
				for (i = 0; i < 10; i++) {//�ҋ@
					if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
						if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
							Reach_check[i] = 1;
							Reach_check[11]++;
						}
					}
				}
				if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {//�Η���������L�W
					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
						if (Connection_Check[i] == 1) {
							Calcu_value[i] = ((-15 * Rotation_Value[2][i]) / (sec * sec * 32)) - ((-15 * Rotation_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}
					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;
							Halt_check[i] = 0;
							VCS_MoveToPosition(keyHandle, NodeId + i, Rotation_Value[0][i], 1, 1, &errorCode);
						}
					}
					Reha_step = 0;
					R_Num++;
					SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				}
			}


			//���n�r���I���̏���
			if (R_Target <= R_Num || Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				KillTimer(hwndSR, 10);
			}
			return 1;

		case 11:
			//�Η� 0�ʒu����3�b
			Test_Calcu[0][2] = ((-15 * Rotation_Value[2][2]) / (3 * 3 * 32)) - ((-15 * Rotation_Value[1][2]) / (3 * 3 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[0][2] < 0) { Test_Calcu[0][2] = Test_Calcu[0][2] * -1; }

			//���]�@0�ʒu������]�ʒu�܂� 4�b
			Test_Calcu[0][0] = ((-15 * Rotation_Value[0][0]) / (5 * 5 * 32)) - ((-15 * Rotation_Value[2][0]) / (5 * 5 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[0][0] < 0) { Test_Calcu[0][0] = Test_Calcu[0][0] * -1; }

			//�O�] 5�b
			Test_Calcu[2][1] = ((-15 * Rotation_Value[1][1]) / (8 * 8 * 32)) - ((-15 * Rotation_Value[0][1]) / (8 * 8 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[2][1] < 0) { Test_Calcu[2][1] = Test_Calcu[2][1] * -1; }

			//����7�b
			Test_Calcu[0][3] = ((-15 * Rotation_Value[1][3]) / (7 * 7 * 32)) - ((-15 * Rotation_Value[0][3]) / (7 * 7 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[0][3] < 0) { Test_Calcu[0][3] = Test_Calcu[0][3] * -1; }

			Test_Calcu[1][3] = ((-15 * 0) / (2 * 2 * 32)) - ((-15 * Rotation_Value[1][3]) / (2 * 2 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[1][3] < 0) { Test_Calcu[1][3] = Test_Calcu[1][3] * -1; }




			//�Η�0�� 2�b
			Test_Calcu[2][2] = ((-15 * Rotation_Value[1][2]) / (4 * 4 * 32)) - ((-15 * Rotation_Value[2][2]) / (4 * 4 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[2][2] < 0) { Test_Calcu[2][2] = Test_Calcu[2][2] * -1; }

			//���]0�ց@1�b
			Test_Calcu[2][0] = ((-15 * Rotation_Value[2][0]) / (2 * 2 * 32)) - ((-15 * Rotation_Value[0][0]) / (2 * 2 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[2][0] < 0) { Test_Calcu[2][0] = Test_Calcu[2][0] * -1; }

			//�O�]0�� 1�b
			Test_Calcu[1][1] = ((-15 * 0) / (2 * 2 * 32)) - ((-15 * Rotation_Value[1][1]) / (2 * 2 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[1][1] < 0) { Test_Calcu[1][1] = Test_Calcu[1][1] * -1; }


			if (test_flag == 0)

			{

				VCS_SetPositionProfile(keyHandle, NodeId + 0, 4500, Test_Calcu[0][0], Test_Calcu[0][0], &errorCode);//���]�@�Η�������]
				VCS_SetPositionProfile(keyHandle, NodeId + 1, 4500, Test_Calcu[2][1], Test_Calcu[2][1], &errorCode);//�O�]  �O�]����Η�
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[0][2], Test_Calcu[0][2], &errorCode);//�Η�  �O�]����Η�
				VCS_SetPositionProfile(keyHandle, NodeId + 3, 4500, Test_Calcu[0][3], Test_Calcu[0][3], &errorCode);//�Η�  �O�]����Η�

				test_flag = 1;

			}

			if (test_flag == 1)
			{

				//	VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);

				VCS_MoveToPosition(keyHandle, NodeId + 2, Rotation_Value[2][2], 1, 1, &errorCode);//�Η�������
				VCS_MoveToPosition(keyHandle, NodeId + 1, Rotation_Value[1][1], 1, 1, &errorCode);//�O�]������
				VCS_MoveToPosition(keyHandle, NodeId + 0, Rotation_Value[0][0], 1, 1, &errorCode);//���]������
				VCS_MoveToPosition(keyHandle, NodeId + 3, Rotation_Value[1][3], 1, 1, &errorCode);//���]������


				test_flag = 2;
			}


			if (test_flag == 2)
			{
				if (VCS_WaitForTargetReached(keyHandle, NodeId + 2, 0, &errorCode)) {

					VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[2][2], Test_Calcu[2][2], &errorCode);//�Η��߂�
					VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);//�Η�������

				/*	if (VCS_WaitForTargetReached(keyHandle, NodeId + 2, 0, &errorCode))
					{
						VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[0][2], Test_Calcu[0][2], &errorCode);//�Η�  �O�]����Η�
						VCS_MoveToPosition(keyHandle, NodeId + 2, Rotation_Value[2][2], 1, 1, &errorCode);//�O�]�ƑΗ�������
					}
					*/
				}

				if (VCS_WaitForTargetReached(keyHandle, NodeId + 0, 0, &errorCode)) {

					VCS_SetPositionProfile(keyHandle, NodeId + 0, 4500, Test_Calcu[2][0], Test_Calcu[2][0], &errorCode);//���]�߂�
					VCS_MoveToPosition(keyHandle, NodeId + 0, 0, 1, 1, &errorCode);//
					/*if (VCS_WaitForTargetReached(keyHandle, NodeId, 0, &errorCode))
					{
						VCS_SetPositionProfile(keyHandle, NodeId + 0, 4500, Test_Calcu[0][0], Test_Calcu[0][0], &errorCode);//���]�@
						VCS_MoveToPosition(keyHandle, NodeId + 0, Rotation_Value[0][0], 1, 1, &errorCode);//���]������
					}*/

				}

				if (VCS_WaitForTargetReached(keyHandle, NodeId + 1, 0, &errorCode) && VCS_WaitForTargetReached(keyHandle, NodeId + 3, 0, &errorCode)) {

					VCS_SetPositionProfile(keyHandle, NodeId + 1, 4500, Test_Calcu[1][1], Test_Calcu[1][1], &errorCode);//�O�]�߂�		
					VCS_SetPositionProfile(keyHandle, NodeId + 3, 4500, Test_Calcu[1][3], Test_Calcu[1][3], &errorCode);//�O�]�߂�

					VCS_MoveToPosition(keyHandle, NodeId + 1, 0, 1, 1, &errorCode);//�Η�������
					VCS_MoveToPosition(keyHandle, NodeId + 3, 0, 1, 1, &errorCode);//�Η�������

				//�O�]4�b�ɕϊ�
					if (VCS_WaitForTargetReached(keyHandle, NodeId + 1, 0, &errorCode) && VCS_WaitForTargetReached(keyHandle, NodeId + 3, 0, &errorCode))
					{
						test_flag = 0;
						//VCS_SetPositionProfile(keyHandle, NodeId + 1, 4500, Test_Calcu[0][1], Test_Calcu[0][1], &errorCode);//�O�]  �O�]����Η�
						//VCS_MoveToPosition(keyHandle, NodeId + 1, Rotation_Value[1][1], 1, 1, &errorCode);//�O�]�ƑΗ�������

					}

				}

			}

			if (Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				counter_flag = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				KillTimer(hwndSR, 11);
			}

			return 1;

		case 12:
			//�Η� 0�ʒu����3�b
			Test_Calcu[0][2] = ((-15 * Rotation_Value[2][2]) / (3 * 3 * 32)) - ((-15 * Rotation_Value[1][2]) / (3 * 3 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[0][2] < 0) { Test_Calcu[0][2] = Test_Calcu[0][2] * -1; }

			//���]�@0�ʒu������]�ʒu�܂� 4�b
			Test_Calcu[0][0] = ((-15 * Rotation_Value[0][0]) / (5 * 5 * 32)) - ((-15 * Rotation_Value[2][0]) / (5 * 5 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[0][0] < 0) { Test_Calcu[0][0] = Test_Calcu[0][0] * -1; }


			//�Η�0�� 2�b
			Test_Calcu[2][2] = ((-15 * Rotation_Value[1][2]) / (4 * 4 * 32)) - ((-15 * Rotation_Value[2][2]) / (4 * 4 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[2][2] < 0) { Test_Calcu[2][2] = Test_Calcu[2][2] * -1; }

			//���]0�ց@1�b
			Test_Calcu[2][0] = ((-15 * Rotation_Value[2][0]) / (2 * 2 * 32)) - ((-15 * Rotation_Value[0][0]) / (2 * 2 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
			if (Test_Calcu[2][0] < 0) { Test_Calcu[2][0] = Test_Calcu[2][0] * -1; }


			if (test_flag == 0)

			{

				VCS_SetPositionProfile(keyHandle, NodeId + 0, 4500, Test_Calcu[0][0], Test_Calcu[0][0], &errorCode);//���]�@�Η�������]
				VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[0][2], Test_Calcu[0][2], &errorCode);//�Η�  �O�]����Η�

				test_flag = 1;

			}

			if (test_flag == 1)
			{

				//	VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);

				VCS_MoveToPosition(keyHandle, NodeId + 2, Rotation_Value[2][2], 1, 1, &errorCode);//�Η�������			
				VCS_MoveToPosition(keyHandle, NodeId + 0, Rotation_Value[0][0], 1, 1, &errorCode);//���]������

				test_flag = 2;
			}


			if (test_flag == 2)
			{
				if (VCS_WaitForTargetReached(keyHandle, NodeId + 2, 0, &errorCode)) {

					VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[2][2], Test_Calcu[2][2], &errorCode);//�Η��߂�
					VCS_MoveToPosition(keyHandle, NodeId + 2, 0, 1, 1, &errorCode);//�Η�������

				}

				if (VCS_WaitForTargetReached(keyHandle, NodeId + 0, 0, &errorCode)) {

					VCS_SetPositionProfile(keyHandle, NodeId + 0, 4500, Test_Calcu[2][0], Test_Calcu[2][0], &errorCode);//���]�߂�			
					VCS_MoveToPosition(keyHandle, NodeId + 0, 0, 1, 1, &errorCode);
					//VCS_SetPositionProfile(keyHandle, NodeId + 2, 4500, Test_Calcu[0][2], Test_Calcu[0][2], &errorCode);//�Η�  �O�]����Η�

				}

				if (VCS_WaitForTargetReached(keyHandle, NodeId + 2, 0, &errorCode) && VCS_WaitForTargetReached(keyHandle, NodeId + 0, 0, &errorCode)) {
					test_flag = 0;
				}


			}



			if (Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				counter_flag = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				KillTimer(hwndSR, 12);
			}

			return 1;

			/*	case 13:
					//�Η� 0�ʒu����3�b
					Test_Calcu[0][2] = ((-15 * Rotation_Value[2][2]) / (3 * 3 * 32)) - ((-15 * Rotation_Value[1][2]) / (3 * 3 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
					if (Test_Calcu[0][2] < 0) { Test_Calcu[0][2] = Test_Calcu[0][2] * -1; }

					//���]�@0�ʒu������]�ʒu�܂� 4�b
					Test_Calcu[0][0] = ((-15 * Rotation_Value[0][0]) / (5 * 5 * 32)) - ((-15 * Rotation_Value[2][0]) / (5 * 5 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
					if (Test_Calcu[0][0] < 0) { Test_Calcu[0][0] = Test_Calcu[0][0] * -1; }


					//�Η�0�� 2�b
					Test_Calcu[2][2] = ((-15 * Rotation_Value[1][2]) / (4 * 4 * 32)) - ((-15 * Rotation_Value[2][2]) / (4 * 4 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
					if (Test_Calcu[2][2] < 0) { Test_Calcu[2][2] = Test_Calcu[2][2] * -1; }

					//���]0�ց@1�b
					Test_Calcu[2][0] = ((-15 * Rotation_Value[2][0]) / (2 * 2 * 32)) - ((-15 * Rotation_Value[0][0]) / (2 * 2 * 32));//������=(15*�ڕW�l)/(�b*�b*32)
					if (Test_Calcu[2][0] < 0) { Test_Calcu[2][0] = Test_Calcu[2][0] * -1; }

					*/


		case 14://�����^��

			if (REPRIHA == 2)//2��
			{

				if (Reha_step == 0)
				{

					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����

						if (rep[0] == 1)//�p�[
						{
							Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);


						}
						else if (rep[0] == 2)//�O�[
						{
							Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 3)
						{
							Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 4)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 5)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 6)
						{
							Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}

					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����

						if (rep[0] == 1)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 2)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 3)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 4)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 5)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 6)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}

					}
				}

				if (Reha_step == 1) {

					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}

					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[1] == 1)
								{

									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 2)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 3)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 4)
								{
									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 5)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 6)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[1] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					for (i = 0; i < 10; i++) {
						VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);
					}
					Reha_step == 0;


				}
			}

			if (REPRIHA == 3)
			{
				if (Reha_step == 0)
				{

					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����

						if (rep[0] == 1)//�p�[
						{
							Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);


						}
						else if (rep[0] == 2)//�O�[
						{
							Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 3)
						{
							Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 4)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 5)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 6)
						{
							Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}

					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����

						if (rep[0] == 1)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 2)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 3)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 4)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 5)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 6)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}

					}
				}

				if (Reha_step == 1) {

					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}

					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[1] == 1)
								{

									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 2)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 3)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 4)
								{
									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 5)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 6)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[1] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 2;

				}

				if (Reha_step == 2)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[2] == 1)
								{

									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 2)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 3)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 4)
								{
									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 5)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 6)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[2] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}
					for (i = 0; i < 10; i++) {
						VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);
					}
					Reha_step == 0;

				}
			}

			if (REPRIHA == 4)
			{
				if (Reha_step == 0)
				{

					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����

						if (rep[0] == 1)//�p�[
						{
							Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);


						}
						else if (rep[0] == 2)//�O�[
						{
							Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 3)
						{
							Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 4)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 5)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 6)
						{
							Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}

					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����

						if (rep[0] == 1)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 2)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 3)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 4)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 5)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 6)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}

					}
				}

				if (Reha_step == 1) {

					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}

					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[1] == 1)
								{

									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 2)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 3)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 4)
								{
									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 5)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 6)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[1] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 2;

				}

				if (Reha_step == 2)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[2] == 1)
								{

									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 2)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 3)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 4)
								{
									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 5)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 6)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[2] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 3;

				}

				if (Reha_step == 3)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[3] == 1)
								{

									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 2)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 3)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 4)
								{
									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 5)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 6)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[3] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}
					for (i = 0; i < 10; i++) {
						VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);
					}
					Reha_step == 0;
				}

			}

			if (REPRIHA == 5)
			{
				if (Reha_step == 0)
				{

					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����

						if (rep[0] == 1)//�p�[
						{
							Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);


						}
						else if (rep[0] == 2)//�O�[
						{
							Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 3)
						{
							Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 4)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 5)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 6)
						{
							Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}

					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����

						if (rep[0] == 1)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 2)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 3)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 4)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 5)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 6)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}

					}
				}

				if (Reha_step == 1) {

					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}

					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[1] == 1)
								{

									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 2)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 3)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 4)
								{
									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 5)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 6)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[1] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 2;

				}

				if (Reha_step == 2)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[2] == 1)
								{

									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 2)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 3)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 4)
								{
									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 5)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 6)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[2] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 3;

				}

				if (Reha_step == 3)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[3] == 1)
								{

									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 2)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 3)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 4)
								{
									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 5)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 6)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[3] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 4;
				}

				if (Reha_step == 4)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[4] == 1)
								{

									if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 2)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 3)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 4)
								{
									if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 5)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 6)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[4] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}
					for (i = 0; i < 10; i++) {
						VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);
					}
					Reha_step == 0;
				}

			}

			if (REPRIHA == 6)
			{
				if (Reha_step == 0)
				{

					for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����

						if (rep[0] == 1)//�p�[
						{
							Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);


						}
						else if (rep[0] == 2)//�O�[
						{
							Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 3)
						{
							Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 4)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 5)
						{
							Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
						else if (rep[0] == 6)
						{
							Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * 0) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
							if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
							VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
						}
					}

					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����

						if (rep[0] == 1)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 2)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 3)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 4)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 5)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}
						else if (rep[0] == 6)
						{
							VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							Reha_step == 1;
						}

					}
				}

				if (Reha_step == 1) {

					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}

					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[1] == 1)
								{

									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 2)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 3)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 4)
								{
									if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 5)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[1] == 6)
								{
									if (rep[0] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[0] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[1] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[1] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 2;

				}

				if (Reha_step == 2)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[2] == 1)
								{

									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 2)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 3)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 4)
								{
									if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 5)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[2] == 6)
								{
									if (rep[1] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[1] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[2] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[2] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 3;

				}

				if (Reha_step == 3)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[3] == 1)
								{

									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 2)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 3)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 4)
								{
									if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 5)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[3] == 6)
								{
									if (rep[2] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[2] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[3] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[3] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 4;
				}

				if (Reha_step == 4)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[4] == 1)
								{

									if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 2)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 3)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 4)
								{
									if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 5)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[4] == 6)
								{
									if (rep[3] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[3] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[4] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[4] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}

					Reha_step == 5;
				}

				if (Reha_step == 5)
				{
					for (i = 0; i < 10; i++) {//�ҋ@
						if (Connection_Check[i] == 1) {//�ڑ����Ă鑕�u�̂�
							if (Reach_check[i] == 0 && VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode)) {
								Reach_check[i] = 1;
								Reach_check[11]++;
							}
						}
					}
					if (Reach_check[10] == Reach_check[11] && Reha_end == 0) {
						for (i = 0; i < 10; i++) {//�ڕW�l������������v�Z���Ďw�肵���b�œ�����
							if (Connection_Check[i] == 1) {

								if (rep[5] == 1)
								{

									if (rep[4] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[5] == 2)
								{
									if (rep[4] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[0][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}

									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[5] == 3)
								{
									if (rep[4] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 5) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[1][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[5] == 4)
								{
									if (rep[4] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 6) {
										Calcu_value[i] = ((-15 * Extension_Value[2][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[5] == 5)
								{
									if (rep[4] == 1) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 2) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 3) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 4) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 6) {
										Calcu_value[i] = ((-15 * Flexion_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
								else if (rep[5] == 6)
								{
									if (rep[4] == 1) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 2) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[0][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 3) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[1][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 4) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Extension_Value[2][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									else if (rep[4] == 5) {
										Calcu_value[i] = ((-15 * Extension_Value[3][i]) / (sec * sec * 32)) - ((-15 * Flexion_Value[3][i]) / (sec * sec * 32));//������=(15*�ڕW�l)/(�b*�b*32)
									}
									if (Calcu_value[i] < 0) { Calcu_value[i] = Calcu_value[i] * -1; }
									VCS_SetPositionProfile(keyHandle, NodeId + i, 4500, Calcu_value[i], Calcu_value[i], &errorCode);
								}
							}
						}
					}

					Reach_check[11] = 0;
					for (i = 0; i < 10; i++) {//�ڕW�l�֓�����
						if (Connection_Check[i] == 1) {
							Reach_check[i] = 0;

							if (rep[5] == 1)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[5] == 2)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[0][i], 1, 1, &errorCode);
							}
							else if (rep[5] == 3)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[1][i], 1, 1, &errorCode);
							}
							else if (rep[5] == 4)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[2][i], 1, 1, &errorCode);
							}
							else if (rep[5] == 5)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Flexion_Value[3][i], 1, 1, &errorCode);
							}
							else if (rep[5] == 6)
							{
								VCS_MoveToPosition(keyHandle, NodeId + i, Extension_Value[3][i], 1, 1, &errorCode);
							}
						}
					}
					for (i = 0; i < 10; i++) {
						VCS_WaitForTargetReached(keyHandle, NodeId + i, 0, &errorCode);
					}
					Reha_step == 0;
				}

			}

			if (Reha_end == 1) {
				//�I�����̏���
				for (i = 0; i < 10; i++) {//�ڕW�l����0�֓�����
					if (Connection_Check[i] == 1) {
						VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode);
						VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
					}
				}
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Fle_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Fle_Reha_Once), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Normal_Ext_Reha), 1);
				EnableWindow(GetDlgItem(hwndSR, IDC_Simple_Sensor_Ext_Reha_Once), 1);
				if (fileopen == 1) {
					fileopen = 0;
					fclose(fp);
				}
				KillTimer(hwndSR, 2);
				KillTimer(hwndSR, 3);
				Reach_check[11] = 0;
				Reha_end = 0;
				Reha_close = 0;
				R_Num = 0;
				Reha_step = 0;
				SetDlgItemInt(hwndSR, IDC_Simple_EDIT2, R_Num, TRUE);
				SetDlgItemText(hwndSR, IDC_Simple_EDIT5, "��~��");
				KillTimer(hwndSR, 12);
			}

			return 1;



		}
		// �����ŏ������Ȃ��R���g���[�����I�����ꂽ�ꍇ��Windows�ɔC���邽�߂�0��Ԃ��B 
		return 0;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE:
		if (Reha_close == 0) {
			if (fileopen == 1) {
				fileopen = 0;
				fclose(fp);
			}
			for (i = 1; i <= 15; i++) {
				KillTimer(hwndSR, i);
			}
			SetTimer(start, 1, 100, NULL);
			R_Num = 0;
			DestroyWindow(hwndSR);
		}
		else {
			MessageBox(hwndSR, "���n�r�����ł��B", "�x��", MB_OK);
		}
		return 1;

	}
	return 0;
}

BOOL CALLBACK DialogS(HWND hwndS, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Connect_BUTTON:
			SetTimer(hwndS, 1, 100, NULL);
			return 1;

			//�V�^�E���^�I��
		case IDC_RADIO1://�V�^
			hwndSS = CreateDialog(singata, "singata", NULL, (DLGPROC)DialogSS);//�_�C�A���O�{�b�N�X���\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndSS == NULL)
			{
				MessageBox(NULL, "�_�C�A���O�{�b�N�X��ID���Ԉ���Ă��܂���?", "�I��", MB_OK);
				return 0;
			}

			//(2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndSS, SW_SHOW);
			UpdateWindow(hwndSS);
			return 1;

		case IDC_RADIO2://���^
			hwndKS = CreateDialog(kyugata, "kyugata", NULL, (DLGPROC)DialogKS);
			//�_�C�A���O�{�b�N�X���\������Ȃ��ꍇ�̓v���O�������I������B
			if (hwndKS == NULL)
			{
				MessageBox(NULL, "�_�C�A���O�{�b�N�X��ID���Ԉ���Ă��܂���?", "�I��", MB_OK);
				return 0;
			}

			//(2)�_�C�A���O�{�b�N�X��\������B
			ShowWindow(hwndKS, SW_SHOW);
			UpdateWindow(hwndKS);
			return 1;

		case IDC_All_Check://�S�ă`�F�b�N
			SendMessage(GetDlgItem(hwndS, IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK2), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK3), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK4), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK5), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK6), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK7), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK8), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK9), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK10), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK11), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK12), BM_SETCHECK, BST_CHECKED, 0);
			return 1;

		case IDC_All_UnCheck://�S�ă`�F�b�N�͂���
			SendMessage(GetDlgItem(hwndS, IDC_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK7), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK8), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK9), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK10), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK11), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndS, IDC_CHECK12), BM_SETCHECK, BST_UNCHECKED, 0);
			return 1;

		}
		return 0;

		//����������
	case WM_INITDIALOG:
		InitCommonControls();
		//MSP�v���O���X�o�[�\��
		SendMessage(GetDlgItem(hwndS, IDC_PROMSP), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROMSP), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROMSP), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��

		//�ؓd�v���O���X�o�[�\��
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN1), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN1), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN1), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��

		SendMessage(GetDlgItem(hwndS, IDC_PROKIN2), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN2), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN2), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��

		SendMessage(GetDlgItem(hwndS, IDC_PROKIN3), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN3), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN3), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��

		SendMessage(GetDlgItem(hwndS, IDC_PROKIN4), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN4), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_PROKIN4), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��

		//�O���[�Z���T�v���O���X�o�[�ݒ�
		//���]
		SendMessage(GetDlgItem(hwndS, IDC_InnerRoll), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_InnerRoll), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_InnerRoll), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		//�O�]																 
		SendMessage(GetDlgItem(hwndS, IDC_OUTERRoll), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_OUTERRoll), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_OUTERRoll), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		//��w����
		SendMessage(GetDlgItem(hwndS, IDC_THUF), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_THUF), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_THUF), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		//���w����														 
		SendMessage(GetDlgItem(hwndS, IDC_INDIFLEX), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_INDIFLEX), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_INDIFLEX), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		//�wor���w����
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLFLEX), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLFLEX), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLFLEX), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		 //���w�L�W
		SendMessage(GetDlgItem(hwndS, IDC_INDIEXTE), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_INDIEXTE), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_INDIEXTE), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		//�wor���w�L�W
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLEXTE), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLEXTE), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLEXTE), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		return 1;

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
			/*case 1: //�V���A���ʐM�̒l�̑���M�Ƃ�
				cserial->m_senddata[0] = 1;		 	// ���얽�߂𑗂�
				cserial->m_senddata[1] = 0;			// �g���ĂȂ�
				cserial->SendSerialData(2);						// �p�\�R������dsPIC��1byte�̃f�[�^�𑗐M
				cserial->ReceiveSerialData(27);				// �p�\�R���ɗ��Ă���V���A���f�[�^��9byte��M

				//�{�^���f�[�^
				Sensor_data[0] = cserial->m_receivedata[1] << 8;
				Sensor_data[0] |= cserial->m_receivedata[0];
				//�f�B�X�N�f�[�^
				Sensor_data[1] = cserial->m_receivedata[3] << 8;
				Sensor_data[1] |= cserial->m_receivedata[2];
				//�ؓd1
				Sensor_data[2] = cserial->m_receivedata[5] << 8;
				Sensor_data[2] |= cserial->m_receivedata[4];
				//�ؓd2
				Sensor_data[3] = cserial->m_receivedata[7] << 8;
				Sensor_data[3] |= cserial->m_receivedata[6];
				//�ؓd3
				Sensor_data[4] = cserial->m_receivedata[9] << 8;
				Sensor_data[4] |= cserial->m_receivedata[8];
				//�ؓd4
				Sensor_data[5] = cserial->m_receivedata[11] << 8;
				Sensor_data[5] |= cserial->m_receivedata[10];
				//���]
				Sensor_data[6] = cserial->m_receivedata[13] << 8;
				Sensor_data[6] |= cserial->m_receivedata[12];
				//�O�]
				Sensor_data[7] = cserial->m_receivedata[15] << 8;
				Sensor_data[7] |= cserial->m_receivedata[14];
				//��w����
				Sensor_data[8] = cserial->m_receivedata[17] << 8;
				Sensor_data[8] |= cserial->m_receivedata[16];
				//���w����
				Sensor_data[9] = cserial->m_receivedata[19] << 8;
				Sensor_data[9] |= cserial->m_receivedata[18];
				//�wor���w����
				Sensor_data[10] = cserial->m_receivedata[21] << 8;
				Sensor_data[10] |= cserial->m_receivedata[20];
				//���w�L�W
				Sensor_data[11] = cserial->m_receivedata[23] << 8;
				Sensor_data[11] |= cserial->m_receivedata[22];
				//�wor���w�L�W
				Sensor_data[12] = cserial->m_receivedata[25] << 8;
				Sensor_data[12] |= cserial->m_receivedata[24];
				//MSP
				Sensor_data[13] = cserial->m_receivedata[26];    //26

			/*	for (i = 2; i < 13; i++) {//�Z���T�̒l��0�`100�ɐݒ�
					Sensor_data[i] = (Sensor_data[i] / 4096) * 100;
				}*/

				//Check�{�^�����ꂽ�Z���T�����l�\��
				//�؎��k�I��
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK1)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_PROMSP), PBM_SETPOS, Sensor_data[13], 0);//MSP
				SetDlgItemInt(hwndS, IDC_SENSOR_EDIT1, Sensor_data[13], 1);//MSP
				//Dig1�v���O���X�o�[�l�\��
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROB), PBM_SETPOS, Sensor_data[0], 0);//�V�^�{�^��
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROD), PBM_SETPOS, Sensor_data[1], 0);//�V�^�f�B�X�N
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROMSP), PBM_SETPOS, Sensor_data[13], 0);//�V�^MSP
				SetDlgItemInt(hwndSS, IDC_DIG1B, Sensor_data[0], 1);//Dialog1�V�^�̃{�^��
				SetDlgItemInt(hwndSS, IDC_DIG1D, Sensor_data[1], 1);//Dialgo1�V�^�̃f�B�X�N
				SetDlgItemInt(hwndSS, IDC_DIG1MSP, Sensor_data[13], 1);//Dialgo1�V�^��MSP
				//Dig2�v���O���X�o�[�l�\��
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROB), PBM_SETPOS, Sensor_data[0], 0);//���^�{�^��
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROD), PBM_SETPOS, Sensor_data[1], 0);//���^�f�B�X�N
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROMSP), PBM_SETPOS, Sensor_data[13], 0);//���^MSP
				SetDlgItemInt(hwndKS, IDC_DIG2B, Sensor_data[0], 1);//Dialog2���^�̃{�^��
				SetDlgItemInt(hwndKS, IDC_DIG2D, Sensor_data[1], 1);//Dialog2���^�̃f�B�X�N
				SetDlgItemInt(hwndKS, IDC_DIG2MSP, Sensor_data[13], 1);//Dialog2���^��MSP
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_PROMSP), PBM_SETPOS, 0, 0);//MSP
				SetDlgItemInt(hwndS, IDC_SENSOR_EDIT1, 0, 1);//MSP
				//Dig1�v���O���X�o�[�l�\��
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROB), PBM_SETPOS, 0, 0);//���^�{�^��
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROD), PBM_SETPOS, 0, 0);//���^�f�B�X�N
				SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROMSP), PBM_SETPOS, 0, 0);//���^MSP
				SetDlgItemInt(hwndSS, IDC_DIG1B, 0, 1);//Dialog1�V�^�̃{�^��
				SetDlgItemInt(hwndSS, IDC_DIG1D, 0, 1);//Dialgo1�V�^�̃f�B�X�N
				SetDlgItemInt(hwndSS, IDC_DIG1MSP, 0, 1);//Dialgo1�V�^��MSP
				//Dig2�v���O���X�o�[�l�\��
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROB), PBM_SETPOS, 0, 0);//���^�{�^��
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROD), PBM_SETPOS, 0, 0);//���^�f�B�X�N
				SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROMSP), PBM_SETPOS, 0, 0);//���^MSP
				SetDlgItemInt(hwndKS, IDC_DIG2B, 0, 1);//Dialog2���^�̃{�^��
				SetDlgItemInt(hwndKS, IDC_DIG2D, 0, 1);//Dialog2���^�̃f�B�X�N
				SetDlgItemInt(hwndKS, IDC_DIG2MSP, 0, 1);//Dialog2���^��MSP
			}

			//�ؓd�I��
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK2)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN1), PBM_SETPOS, Sensor_data[2], 0);//�ؓd1
				SetDlgItemInt(hwndS, IDC_KINDEN1, Sensor_data[2], 1);//�ؓd1
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN1), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_KINDEN1, 0, 1);//�ؓd1
			}

			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK3)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN2), PBM_SETPOS, Sensor_data[3], 0);//�ؓd2
				SetDlgItemInt(hwndS, IDC_KINDEN2, Sensor_data[3], 1);//�ؓd2
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN2), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_KINDEN2, 0, 1);//�ؓd2
			}

			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK4)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN3), PBM_SETPOS, Sensor_data[4], 0);//�ؓd3
				SetDlgItemInt(hwndS, IDC_KINDEN3, Sensor_data[4], 1);//�ؓd3
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN3), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_KINDEN3, 0, 1);//�ؓd3
			}

			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK5)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN4), PBM_SETPOS, Sensor_data[5], 0);//�ؓd4
				SetDlgItemInt(hwndS, IDC_KINDEN4, Sensor_data[5], 1);//�ؓd4
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_PROKIN4), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_KINDEN4, 0, 1);//�ؓd4
			}

			//�O���[�Z���T�I��
			//���]
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK6)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_InnerRoll), PBM_SETPOS, Sensor_data[6], 0);//���]
				SetDlgItemInt(hwndS, IDC_EINNER, Sensor_data[6], 1);//���]
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_InnerRoll), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_EINNER, 0, 1);//���]
			}
			//�O�]
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK7)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_OUTERRoll), PBM_SETPOS, Sensor_data[7], 0);//�O�]
				SetDlgItemInt(hwndS, IDC_EOUTER, Sensor_data[7], 1);//�O�]
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_OUTERRoll), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_EOUTER, 0, 1);//�O�]
			}
			//��w����
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK8)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_THUF), PBM_SETPOS, Sensor_data[8], 0);//��w����
				SetDlgItemInt(hwndS, IDC_ETHUF, Sensor_data[8], 1);//��w����
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_THUF), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_ETHUF, 0, 1);//��w����
			}
			//���w����
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK9)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_INDIFLEX), PBM_SETPOS, Sensor_data[9], 0);//���w����
				SetDlgItemInt(hwndS, IDC_EINDIF, Sensor_data[9], 1);//���w����
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_INDIFLEX), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_EINDIF, 0, 1);//���w����
			}
			//�wor���w����
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK10)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLFLEX), PBM_SETPOS, Sensor_data[10], 0);//�wor���w����
				SetDlgItemInt(hwndS, IDC_ERorSF, Sensor_data[10], 1);//�w�A���w����
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLFLEX), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_ERorSF, 0, 1);//�w�A���w����
			}
			//���w�L�W
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK11)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_INDIEXTE), PBM_SETPOS, Sensor_data[11], 0);//���w�L�W
				SetDlgItemInt(hwndS, IDC_EINDIE, Sensor_data[11], 1);//���w�L�W
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_INDIEXTE), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_EINDIE, 0, 1);//���w�L�W
			}
			//�wor���w�L�W
			if (Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK12)) == BST_CHECKED) {
				SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLEXTE), PBM_SETPOS, Sensor_data[12], 0);//�wor���w�L�W
				SetDlgItemInt(hwndS, IDC_ERorSE, Sensor_data[12], 1);//�w�A���w�L�W
			}
			else {
				SendMessage(GetDlgItem(hwndS, IDC_RINGORSMALLEXTE), PBM_SETPOS, 0, 0);
				SetDlgItemInt(hwndS, IDC_ERorSE, 0, 1);//�w�A���w�L�W
			}
			return 1;
		}
		// �����ŏ������Ȃ��R���g���[�����I�����ꂽ�ꍇ��Windows�ɔC���邽�߂�0��Ԃ��B 
		return 0;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE:
		//�g�p����Z���T�[��ۑ�
		Used_Sensor[0] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK1));
		Used_Sensor[1] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK2));
		Used_Sensor[2] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK3));
		Used_Sensor[3] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK4));
		Used_Sensor[4] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK5));
		Used_Sensor[5] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK6));
		Used_Sensor[6] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK7));
		Used_Sensor[7] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK8));
		Used_Sensor[8] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK9));
		Used_Sensor[9] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK10));
		Used_Sensor[10] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK11));
		Used_Sensor[11] = Button_GetCheck(GetDlgItem(hwndS, IDC_CHECK12));
		KillTimer(hwndS, 1);
		SetTimer(start, 1, 100, NULL);
		DestroyWindow(hwndS);
		return 1;

	}
	return 0;
}

//�V�^�؎��k�Z���T

BOOL CALLBACK DialogSS(HWND hwndSS, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//������
		case IDC_MUHUKAS:
			cserial->m_senddata[1] = 2;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�ߕ��׃{�^��
		case IDC_KAHUKAB:
			cserial->m_senddata[1] = 4;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�ߕ��׃f�B�X�N
		case IDC_KAHUKAD:
			cserial->m_senddata[1] = 5;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�����b�N�X
		case IDC_RELAXS:
			cserial->m_senddata[1] = 6;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�͓����
		case IDC_MAXIMS:
			cserial->m_senddata[1] = 7;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//MSP�\��
		case IDC_MSPS:
			cserial->m_senddata[1] = 8;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;

		}
		return 0;

	case WM_INITDIALOG:
		//�{�^��
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROB), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROB), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROB), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
																		//�f�B�X�N
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROD), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROD), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROD), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
																		//MSP
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROMSP), PBM_SETRANGE, 0, MAKELPARAM(0, 100));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROMSP), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndSS, IDC_DIG1PROMSP), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		return 1;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE:
		DestroyWindow(hwndSS);
		return 1;
	}
	return 0;
}

//���^�؍d�x�ݒ�
BOOL CALLBACK DialogKS(HWND hwndKS, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		//�R���g���[���ɉ����������ۂɎ��s
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//�؍d�x�m�[�}���C�Y
			//������
		case IDC_MUHUKA1:
			cserial->m_senddata[1] = 2;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�ߕ��׃{�^��
		case IDC_KAHUKA1:
			cserial->m_senddata[1] = 3;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�����b�N�X
		case IDC_RELAX1:
			cserial->m_senddata[1] = 6;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//�͓����
		case IDC_MAXIM1:
			cserial->m_senddata[1] = 7;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
			//MSP�\��
		case IDC_MSPK:
			cserial->m_senddata[1] = 8;
			cserial->SendSerialData(2);
			cserial->ReceiveSerialData(27);
			return 1;
		}
		return 0;

	case WM_INITDIALOG:
		//�{�^��
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROB), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROB), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROB), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
																		//�f�B�X�N
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROD), PBM_SETRANGE, 0, MAKELPARAM(0, 4096));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROD), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROD), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
																		//MSP
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROMSP), PBM_SETRANGE, 0, MAKELPARAM(0, 100));//�v���O���X�o�[�͈̔͂̐ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROMSP), PBM_SETPOS, 0, 0);//�v���O���X�o�[�̈ʒu�ݒ�
		SendMessage(GetDlgItem(hwndKS, IDC_DIG2PROMSP), PBM_SETSTEP, 0, 0);//�X�e�b�v����ݒ肷��
		return 1;

		// �~�{�^�����N���b�N�����Ƃ��̏���
	case WM_CLOSE:
		DestroyWindow(hwndKS);
		return 1;
	}
	return 0;

}

//��������I��
BOOL CALLBACK DialogRS(HWND hwndRS, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_R1st1://1st�S�w�L�W
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_CHECKED)
			{
				rep[0] = 1;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);


				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd1)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd1), BM_SETCHECK, BST_UNCHECKED, 0);
				}


			}
			return 1;
		case IDC_R1st2://�S�w����
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_CHECKED)
			{
				rep[0] = 2;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);


				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd2)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd2), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
		case IDC_R1st3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_CHECKED)
			{
				rep[0] = 3;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);

				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd3)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R1st4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_CHECKED)
			{
				rep[0] = 4;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);

				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd4)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd4), BM_SETCHECK, BST_UNCHECKED, 0);
				}


			}
			return 1;

		case IDC_R1st5:

			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_CHECKED)
			{

				rep[0] = 5;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);


				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd5)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd5), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}


			return 1;

		case IDC_R1st6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_CHECKED)
			{
				rep[0] = 6;
				rep_det[0] = 1;

				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 0);


				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd6)) == BST_CHECKED)
				{
					rep_det[1] = 0;
					rep[1] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R2nd6), BM_SETCHECK, BST_UNCHECKED, 0);
				}


			}
			return 1;

		case IDC_R2nd1://1st�S�w�L�W
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd1)) == BST_CHECKED)
			{
				rep[1] = 1;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd1)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd1), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R2nd2:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd2)) == BST_CHECKED)
			{
				rep[1] = 2;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd2)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd2), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R2nd3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd3)) == BST_CHECKED)
			{
				rep[1] = 3;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd3)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R2nd4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd4)) == BST_CHECKED)
			{
				rep[1] = 4;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd4)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd4), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R2nd5:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd5)) == BST_CHECKED)
			{
				rep[1] = 5;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd5)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd5), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R2nd6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R2nd6)) == BST_CHECKED)
			{
				rep[1] = 6;
				rep_det[1] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd6)) == BST_CHECKED)
				{
					rep_det[2] = 0;
					rep[2] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R3rd6), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd1:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd1)) == BST_CHECKED)
			{
				rep[2] = 1;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th1)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th1), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd2:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd2)) == BST_CHECKED)
			{
				rep[2] = 2;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th2)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th2), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd3)) == BST_CHECKED)
			{
				rep[2] = 3;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th3)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd4)) == BST_CHECKED)
			{
				rep[2] = 4;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th4)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th4), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd5:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd5)) == BST_CHECKED)
			{
				rep[2] = 5;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th5)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th5), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R3rd6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R3rd6)) == BST_CHECKED)
			{
				rep[2] = 6;
				rep_det[2] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th6)) == BST_CHECKED)
				{
					rep_det[3] = 0;
					rep[3] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R4th6), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th1:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th1)) == BST_CHECKED)
			{
				rep[3] = 1;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th1)) == BST_CHECKED)
				{
					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th1), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th2:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th2)) == BST_CHECKED)
			{
				rep[3] = 2;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th2)) == BST_CHECKED)
				{
					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th2), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th3)) == BST_CHECKED)
			{
				rep[3] = 3;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th3)) == BST_CHECKED)
				{

					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th4)) == BST_CHECKED)
			{
				rep[3] = 4;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th4)) == BST_CHECKED)
				{

					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th4), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th5:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th5)) == BST_CHECKED)
			{
				rep[3] = 5;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 0);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th5)) == BST_CHECKED)
				{

					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th5), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R4th6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R4th6)) == BST_CHECKED)
			{
				rep[3] = 6;
				rep_det[3] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
				EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th6)) == BST_CHECKED)
				{

					rep_det[4] = 0;
					rep[4] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R5th6), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th1:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th1)) == BST_CHECKED)
			{
				rep[4] = 1;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th1)) == BST_CHECKED)
				{

					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th1), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th2:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th2)) == BST_CHECKED)
			{
				rep[4] = 2;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th2)) == BST_CHECKED)
				{

					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th2), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th3)) == BST_CHECKED)
			{
				rep[4] = 3;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th3)) == BST_CHECKED)
				{

					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th4)) == BST_CHECKED)
			{
				rep[4] = 4;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th3)) == BST_CHECKED)
				{

					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th3), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th5:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th5)) == BST_CHECKED)
			{
				rep[4] = 5;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st6)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th5)) == BST_CHECKED)
				{

					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th5), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R5th6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R5th6)) == BST_CHECKED)
			{
				rep[4] = 6;
				rep_det[4] = 1;
				EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 0);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st1)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st2)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st3)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st4)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R1st5)) == BST_UNCHECKED)EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
				if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th6)) == BST_CHECKED)
				{
					rep_det[5] = 0;
					rep[5] = 0;
					SendMessage(GetDlgItem(hwndRS, IDC_R6th6), BM_SETCHECK, BST_UNCHECKED, 0);
				}

			}
			return 1;

		case IDC_R6th1:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th1)) == BST_CHECKED) {
				rep[5] = 1;
				rep_det[5] = 1;
			}

			return 1;

		case IDC_R6th2:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th2)) == BST_CHECKED) {
				rep[5] = 2;
				rep_det[5] = 1;
			}

			return 1;

		case IDC_R6th3:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th3)) == BST_CHECKED) {
				rep[5] = 3;
				rep_det[5] = 1;
			}

			return 1;

		case IDC_R6th4:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th4)) == BST_CHECKED) {
				rep[5] = 4;
				rep_det[5] = 1;
			}

			return 1;

		case IDC_R6th5:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th5)) == BST_CHECKED) {
				rep[5] = 5;
				rep_det[5] = 1;
			}

			return 1;

		case IDC_R6th6:
			if (Button_GetCheck(GetDlgItem(hwndRS, IDC_R6th6)) == BST_CHECKED) {
				rep[5] = 6;
				rep_det[5] = 1;
			}

			return 1;
		case IDC_Breset1:

			rep_det[0] = 0;
			rep[0] = 0;

			SendMessage(GetDlgItem(hwndRS, IDC_R1st1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st6), BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);

			return 1;

		case IDC_Breset2:

			rep_det[1] = 0;
			rep[1] = 0;

			SendMessage(GetDlgItem(hwndRS, IDC_R2nd1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd6), BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);

			return 1;

		case IDC_Breset3:

			rep_det[2] = 0;
			rep[2] = 0;
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd6), BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);

			return 1;

		case IDC_Breset4:

			rep_det[3] = 0;
			rep[3] = 0;
			SendMessage(GetDlgItem(hwndRS, IDC_R4th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th6), BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);

			return 1;

		case IDC_Breset5:

			rep_det[4] = 0;
			rep[4] = 0;
			SendMessage(GetDlgItem(hwndRS, IDC_R5th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th6), BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);

			return 1;

		case IDC_Breset6:

			rep_det[5] = 0;
			rep[5] = 0;
			SendMessage(GetDlgItem(hwndRS, IDC_R6th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th6), BM_SETCHECK, BST_UNCHECKED, 0);

			return 1;

		case IDC_RPreset:

			SendMessage(GetDlgItem(hwndRS, IDC_R1st1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R1st6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R2nd6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R3rd6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R4th6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R5th6), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th1), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th2), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th3), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th4), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th5), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwndRS, IDC_R6th6), BM_SETCHECK, BST_UNCHECKED, 0);

			EnableWindow(GetDlgItem(hwndRS, IDC_R1st1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R1st2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R1st3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R1st4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R1st5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R1st6), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R2nd6), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R3rd6), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R4th6), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R5th6), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th1), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th2), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th3), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th4), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th5), 1);
			EnableWindow(GetDlgItem(hwndRS, IDC_R6th6), 1);

			for (i = 0; i < 6; i++)
			{
				rep_det[i] = 0;
				rep[i] = 0;
			}

			return 1;

		case IDC_RPdet://�����񐔔��f


			if (rep_det[0] == 1 && rep_det[1] == 1 && rep_det[2] == 1 && rep_det[3] == 1 && rep_det[4] == 1 && rep_det[5] == 1)//�U���
			{
				MessageBox(hwndRS, "6��̔����ł��B��낵���ł����H", "�m�F", MB_OKCANCEL);
				REPRIHA = 6;

				if (IDOK)
				{

				}

			}
			else if (rep_det[0] == 1 && rep_det[1] == 1 && rep_det[2] == 1 && rep_det[3] == 1 && rep_det[4] == 1 && rep_det[5] == 0)//5���
			{
				MessageBox(hwndRS, "5��̔����ł��B��낵���ł����H", "�m�F", MB_OKCANCEL);
				REPRIHA = 5;
			}
			else if (rep_det[0] == 1 && rep_det[1] == 1 && rep_det[2] == 1 && rep_det[3] == 1 && rep_det[4] == 0 && rep_det[5] == 0)//5���
			{
				MessageBox(hwndRS, "4��̔����ł��B��낵���ł����H", "�m�F", MB_OKCANCEL);
				REPRIHA = 4;
			}
			else if (rep_det[0] == 1 && rep_det[1] == 1 && rep_det[2] == 1 && rep_det[3] == 0 && rep_det[4] == 0 && rep_det[5] == 0)//5���
			{
				MessageBox(hwndRS, "3��̔����ł��B��낵���ł����H", "�m�F", MB_OKCANCEL);
				REPRIHA = 3;
			}
			else if (rep_det[0] == 1 && rep_det[1] == 1 && rep_det[2] == 0 && rep_det[3] == 0 && rep_det[4] == 0 && rep_det[5] == 0)//5���
			{
				MessageBox(hwndRS, "2��̔����ł��B��낵���ł����H", "�m�F", MB_OKCANCEL);
				REPRIHA = 2;
			}
			else
			{
				MessageBox(hwndRS, "���͂��Ԉ���Ă��܂��B��蒼���ĉ������B", "�x��", MB_OK);

			}

			return 1;





		}
		return 0;

	case WM_INITDIALOG:





		return 1;

	case WM_CLOSE:
		for (i = 0; i < 6; i++)
		{
			rep[i] = 0;
			rep_det[i] = 0;
		}
		DestroyWindow(hwndRS);
		return 1;


	}
	return 0;
}
//LeapMotion
/*static void OnConnect();
static void OnDevice(const LEAP_DEVICE_INFO* props);
static void OnFrame(const LEAP_TRACKING_EVENT* frame);
static void OnImage(const LEAP_IMAGE_EVENT* image);
static void OnLogMessage(const eLeapLogSeverity severity, const int64_t timestamp,
	const char* message);
static void* allocate(uint32_t size, eLeapAllocatorType typeHint, void* state);
static void deallocate(void* ptr, void* state);
void OnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT* change);
void OnHeadPose(const LEAP_HEAD_POSE_EVENT* event);
static LEAP_CONNECTION* connectionHandle; //LEAP_CONNECTION:Leap�ڑ��I�u�W�F�N�g�ւ̃n���h��
*/

static LEAP_CONNECTION* connectionHandle; //LEAP_CONNECTION:Leap�ڑ��I�u�W�F�N�g�ւ̃n���h��
int64_t lastFrameID = 0; //The last frame received
//LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();

	//Onconnect�ɂ͊֘A�f�[�^�͂Ȃ����A�C�x���g���g�p���Đڑ��Ɉˑ�����A�v���̕������������ł���
	/** Callback for when the connection opens. */
static void OnConnect() {
	printf("Connected.\n");
}

//OnDevice�͐ڑ����ꂽ�f�o�C�X���������LEAP_DEVICE_INFO�\���̂�񋟂���
/** Callback for when a device is found. */
static void OnDevice(const LEAP_DEVICE_INFO* props) { //LEAP_DEVICE_INFO:�f�o�C�X�̃n���h�����w�肵��LeapGetDeviceInfo()���Ăяo���ALEAP_DEVICE_INFO���擾����
	printf("Found device %s.\n", props->serial);
}

//OnFrame�͍ŐV�t���[���̂��ׂẴg���b�L���O�f�[�^�i�摜�������j���܂�LEAP_TRACKING_EVENT�\���̂�񋟂���
/** Callback for when a frame of tracking data is available. */
static void OnFrame(const LEAP_TRACKING_EVENT* frame) { //LEAP_TRACKING_EVENT:����u�Ԃ̃g���b�L���O�f�[�^���܂ރf�[�^�̃t���[��

	printf("Frame %lli with %i hands.\n", (long long int)frame->info.frame_id, frame->nHands);
	

	for (uint32_t h = 0; h < frame->nHands; h++) { //nHands:���̃t���[���Ńg���b�L���O���ꂽ��̐���pHands�z��̗v�f�̐�
		LEAP_HAND* hand = &frame->pHands[h]; //LEAP_HAND:�g���b�L���O���ꂽ����������B  pHands;���̃t���[���Ńg���b�L���O������̔z��ւ̃|�C���^
		
		grab = (hand->grab_angle) * (100 / PI);

		printf("grab %d %% \n", grab);//�Ȃ����\���@180���̎�100%,0���̎�0%

		
		printf("grab %d ,%.1f\n", grab, hand->grab_strength);//�O���u�n���h�|�[�Y�̎w�Ǝ�̊Ԃ̊p�x�i0-180���ŕ\���j180���O�[�A0���p�[
		

		if (grab >= 0 && grab <= 100) {

			VCS_MoveToPosition(keyHandle, NodeId + 4, (96 + (0.27 * grab)) * -256, 1, 1, &errorCode);//�d�w����
			VCS_MoveToPosition(keyHandle, NodeId + 5, (96 + (0.49 * grab)) * -256, 1, 1, &errorCode);//�w������
			VCS_MoveToPosition(keyHandle, NodeId + 6, (140 + (0.40* grab)) * -256, 1, 1, &errorCode);//3�w����

			VCS_MoveToPosition(keyHandle, NodeId + 7, (137-(0.12+grab)) * -256, 1, 1, &errorCode);//�d�w�L�W                                                                                                                                           
			VCS_MoveToPosition(keyHandle, NodeId + 8, (180-(0.19*grab)) * -256, 1, 1, &errorCode);//�w���L�W
			VCS_MoveToPosition(keyHandle, NodeId + 9, (137-(0.31*grab)) * -256, 1, 1, &errorCode);//3�w�L�W

			/*if (grab >= 0 && grab <= 10) {
				VCS_MoveToPosition(keyHandle, NodeId + 4, 80 * -256, 1, 1, &errorCode);//�d�w����
				VCS_MoveToPosition(keyHandle, NodeId + 5, 90 * -256, 1, 1, &errorCode);//�w������
				VCS_MoveToPosition(keyHandle, NodeId + 6, 125 * -256, 1, 1, &errorCode);//3�w����

				VCS_MoveToPosition(keyHandle, NodeId + 7, (135 - grab) * -256, 1, 1, &errorCode);//�d�w�L�W                                                                                                                                           
				VCS_MoveToPosition(keyHandle, NodeId + 8, (180 - grab) * -256, 1, 1, &errorCode);//�w���L�W
				VCS_MoveToPosition(keyHandle, NodeId + 9, (132 - grab) * -256, 1, 1, &errorCode);//3�w�L�W
			}*/

		}
	}
	
}

static void OnImage(const LEAP_IMAGE_EVENT* image) { //LEAP_IMAGE_EVENT:�f�o�C�X����X�e���I�摜�y�A���X�g���[�~���O����B
	printf("Image %lli  => Left: %d x %d (bpp=%d), Right: %d x %d (bpp=%d)\n",
		(long long int)image->info.frame_id,
		image->image[0].properties.width, image->image[0].properties.height, image->image[0].properties.bpp * 8,
		image->image[1].properties.width, image->image[1].properties.height, image->image[1].properties.bpp * 8);
}

static void OnLogMessage(const eLeapLogSeverity severity, const int64_t timestamp,
	const char* message) {
	const char* severity_str;
	switch (severity) {
	case eLeapLogSeverity_Critical:
		severity_str = "Critical";
		break;
	case eLeapLogSeverity_Warning:
		severity_str = "Warning";
		break;
	case eLeapLogSeverity_Information:
		severity_str = "Info";
		break;
	default:
		severity_str = "";
		break;
	}
	printf("[%s][%lli] %s\n", severity_str, (long long int)timestamp, message);
}

static void* allocate(uint32_t size, eLeapAllocatorType typeHint, void* state) {
	void* ptr = malloc(size);
	return ptr;
}

static void deallocate(void* ptr, void* state) {
	if (!ptr)
		return;
	free(ptr);
}

void OnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT* change) { //LEAP_POINT_MAPPING_CHANGE_EVENT:�f�o�C�X�̃|�C���g�}�b�s���O���ύX���ꂽ�Ƃ����ʒm
	if (!connectionHandle)
		return;

	uint64_t size = 0;
	if (LeapGetPointMappingSize(*connectionHandle, &size) != eLeapRS_Success || !size)
		return;

	LEAP_POINT_MAPPING* pointMapping = (LEAP_POINT_MAPPING*)malloc(size); //LEAP_POINT_MAPPING:�f�o�C�X�̃|�C���g�}�b�s���O���ύX���ꂽ�Ƃ����ʒm�B����ɂ̓}�b�v����Ă���|�C���g�̃Z�b�g�S�̂��܂܂�Ă���B
	if (!pointMapping)
		return;

	if (LeapGetPointMapping(*connectionHandle, pointMapping, &size) == eLeapRS_Success &&
		pointMapping->nPoints > 0) {
		printf("Managing %u points as of frame %lld at %lld\n", pointMapping->nPoints, (long long int)pointMapping->frame_id, (long long int)pointMapping->timestamp);
	}
	free(pointMapping);
}

void OnHeadPose(const LEAP_HEAD_POSE_EVENT* event) {
	printf("Head pose:\n");
	printf("    Head position (%f, %f, %f).\n",
		event->head_position.x,
		event->head_position.y,
		event->head_position.z);
	printf("    Head orientation (%f, %f, %f, %f).\n",
		event->head_orientation.w,
		event->head_orientation.x,
		event->head_orientation.y,
		event->head_orientation.z);
}



BOOL CALLBACK DialogLeap(HWND hwndL, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON2://�I��
			KillTimer(hwnd, 1);
			SetTimer(start, 1, 100, NULL);
			DestroyWindow(hwnd);
			return	1;

		case IDC_BUTTON1://�ꊇ������
			for (i = 0; i <= 9; i++) {
				VCS_ClearFault(keyHandle, NodeId + i, &errorCode);
				if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_HOMING_MODE, &errorCode))//���_�X�C�b�`�ɓd��������Ƃ��̈ʒu��0�ɂȂ�
				{
					if (VCS_SetHomingParameter(keyHandle, NodeId + i, 10000, 1000, 1, 0, 500, 0, &errorCode))
					{
						if (VCS_SetEnableState(keyHandle, NodeId + i, &errorCode))
						{
							VCS_FindHome(keyHandle, NodeId + i, HM_HOME_SWITCH_POSITIVE_SPEED, &errorCode);
							SetTimer(hwnd, 2 + i, 100, NULL);
						}
					}
				}
			}
			return 1;
			/*case IDC_BUTTON3://�ꊇ�ڑ�
				for (i = 0; i <= 9; i++) {
					if (IsWindowEnabled(GetDlgItem(hwndL, IDC_Connection1 + i))) {//�{�^���̏�Ԃ��P�Ȃ�ɂ�����
						Connection_Check[i] = 1;
						EnableWindow(GetDlgItem(hwndL, IDC_Disconnection1 + i), 1);
						EnableWindow(GetDlgItem(hwndL, IDC_Connection1 + i), 0);
						EnableWindow(GetDlgItem(hwndL, IDC_RESET1 + i), 1);
						if (VCS_SetOperationMode(keyHandle, NodeId + i, OMD_PROFILE_POSITION_MODE, &errorCode))
						{
							if (VCS_SetPositionProfile(keyHandle, NodeId + i, 2400, 3200, 3200, &errorCode))
							{
								if (VCS_SetEnableState(keyHandle, NodeId + i, &errorCode))
								{
									VCS_MoveToPosition(keyHandle, NodeId + i, 0, 1, 1, &errorCode);
									SendMessage(GetDlgItem(hwndL, IDC_SLIDER1 + i), TBM_SETPOS, TRUE, 0); // �X���C�_�[�R���g���[���̈ʒu�̐ݒ�
								}
							}
						}
					}
				}
				return 1;*/

		case IDC_BUTTON4://�O�[,�p�[

			AllocConsole();//�R���\�[���\��
			freopen("CONOUT$", "w", stdout);//�W���o�͂��R���\�[���ɂ���
			freopen("CONIN$", "r", stdin);//�W�����͂��R���\�[���ɂ���
			

				//int main(int argc, char** argv){

					//Set callback function pointers
			ConnectionCallbacks.on_connection = &OnConnect;
			ConnectionCallbacks.on_device_found = &OnDevice;
			ConnectionCallbacks.on_frame = &OnFrame;
			ConnectionCallbacks.on_image = &OnImage;
			ConnectionCallbacks.on_point_mapping_change = &OnPointMappingChange;
			ConnectionCallbacks.on_log_message = &OnLogMessage;
			ConnectionCallbacks.on_head_pose = &OnHeadPose;


			connectionHandle = OpenConnection();
			{
				LEAP_ALLOCATOR allocator = { allocate, deallocate, NULL }; //LEAP_ALLOGATOR:���C�u�������������𓮓I�ɊǗ�����K�v������ꍇ�Ɏg�p�����A���P�[�^/�ŃA���P�[�^�֐����w�肷��
				LeapSetAllocator(*connectionHandle, &allocator);
			}
			LeapSetPolicyFlags(*connectionHandle, eLeapPolicyFlag_Images | eLeapPolicyFlag_MapPoints, 0);

			printf("Press Enter to exit program.\n");
			getchar();

		
			DestroyConnection();
			FreeConsole();

			//return 0;


		//}


	//cserial->ReceiveSerialData(1);
	//a = cserial->m_receivedata[0];
	//b = GetDlgItemInt(hwndL, IDC_EDIT1, NULL, TRUE);

	//VCS_MoveToPosition(keyHandle, NodeId, grab * -256, 1, 1, &errorCode);
	//SetDlgItemInt(hwndL, IDC_EDIT7, grab, TRUE);

			return 1;

		case IDC_BUTTON5://�w�����ȁE�L�W
		//a = GetDlgItemInt(hwndL, IDC_EDIT2, NULL, TRUE);
			//VCS_MoveToPosition(keyHandle, NodeId, c * -256, 1, 1, &errorCode);
			//SetDlgItemInt(hwndL, IDC_EDIT8, grab, TRUE);

			AllocConsole();//�R���\�[���\��
			freopen("CONOUT$", "w", stdout);//�W���o�͂��R���\�[���ɂ���
			freopen("CONIN$", "r", stdin);//�W�����͂��R���\�[���ɂ���

			//int64_t lastFrameID = 0; //The last frame received

			OpenConnection();
			while (!IsConnected)
				millisleep(100); //wait a bit to let the connection complete(�ڑ������܂ő҂�)

			printf("Connected.");
			//LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();
			//if (deviceProps)
				//printf("Using device %s.\n", deviceProps->serial);

			for (;;) {
				LEAP_TRACKING_EVENT* frame = GetFrame();
				if (frame && (frame->tracking_frame_id > lastFrameID)) {
					lastFrameID = frame->tracking_frame_id;
					printf("Frame %lli with %i hands.\n", (long long int)frame->tracking_frame_id, frame->nHands);
					for (uint32_t h = 0; h < frame->nHands; h++) {
						LEAP_HAND* hand = &frame->pHands[h];
						/* printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
									 hand->id,
									 (hand->type == eLeapHandType_Left ? "left" : "right"),
									 hand->palm.position.x,
									 hand->palm.position.y,
									 hand->palm.position.z);*/

						LEAP_DIGIT* finger = &frame->pHands[h].index;
						
						double a = hand->palm.position.z;//��̂Ђ�̈ʒu
						double b = finger->distal.prev_joint.z;//�w�̐�[�̈ʒu
						double c = 0;
						
						printf("index distal position is %f\n", finger->distal.prev_joint.z);//�l�w���w�̐�[�̍��̈ʒu
						//printf("index intermediate position is %f\n",finger->intermediate.prev_joint.z);//�l�����w��P�֐߂̐�[�ʒu
						printf("palm position is %f\n", hand->palm.position.z);


						if ((a < 0 && b < 0) && a > b) {//�w����̂Ђ�̈ʒu���}�C�i�X����̂Ђ�̈ʒu�̕����[���ɋ߂���
							c = (-b) - (-a);
							printf("index-palm distance is %f\n", c);
							
							if (c >= 0 && c < 1) {
								printf("100%%\n");
								
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145-c) * -256, 1, 1, &errorCode);//�w������
								                                                                                                                                      
								VCS_MoveToPosition(keyHandle, NodeId + 8, (161-c) * -256, 1, 1, &errorCode);//�w���L�W
								
							}
							else if (c >= 80) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, 96 * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, 180 * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 1 && c < 80) {
								int d = c / 0.8;
								printf("%d%%\n", 100 - d);
								int k = 100 - d;
								VCS_MoveToPosition(keyHandle, NodeId + 5, (96 + (0.49 * k)) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (180 - (0.19 * k)) * -256, 1, 1, &errorCode);//�w���L�W
							}
						}

						else if ((a < 0 && b < 0) && a < b) {//�w����̂Ђ�̈ʒu���}�C�i�X���w�̈ʒu�̕����[���ɋ߂���
							c = (-a) - (-b);
							printf("index-palm distance is %f\n", c);
							printf("100%%\n");

							VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

							VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
						}

						else if (a < 0 && b>0) {//��̂Ђ�̈ʒu���}�C�i�X�Ŏw�̈ʒu���v���X�̎�
							c = b - (-a);
							printf("index-palm distance is %f\n", c);
							if (a < b) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 0 && c < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 80) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, 96 * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, 180 * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 1 && c < 80) {
								int d = c / 0.8;
								printf("%d%%\n", 100 - d);
								int k = 100 - d;
								VCS_MoveToPosition(keyHandle, NodeId + 5, (96 + (0.49 * k)) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (180 - (0.19 * k)) * -256, 1, 1, &errorCode);//�w���L�W
							}

						}

						else if (a > 0 && b < 0) {//�w�ʒu���}�C�i�X�Ŏ�̂Ђ�ʒu���v���X�̎�
							c = (-b) + a;
							printf("index-palm distance is %f\n", c);
							if (a < b) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 0 && c < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 80) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, 96 * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, 180 * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 1 && c < 80) {
								int d = c / 0.8;
								printf("%d%%\n", 100 - d);
								int k = 100 - d;
								VCS_MoveToPosition(keyHandle, NodeId + 5, (96 + (0.49 * k)) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (180 - (0.19 * k)) * -256, 1, 1, &errorCode);//�w���L�W
							}
						}

						else {
							c = -(b - a);
							printf("index-palm distance is %f\n", c);
							if (a < b) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 0 && c < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, (145 - c) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (161 - c) * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 80) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 5, 96 * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, 180 * -256, 1, 1, &errorCode);//�w���L�W
							}
							else if (c >= 1 && c < 80) {
								int d = c / 0.8;
								printf("%d%%\n", 100 - d);
								int k = 100 - d;
								VCS_MoveToPosition(keyHandle, NodeId + 5, (96 + (0.49 * k)) * -256, 1, 1, &errorCode);//�w������

								VCS_MoveToPosition(keyHandle, NodeId + 8, (180 - (0.19 * k)) * -256, 1, 1, &errorCode);//�w���L�W
							}
						}
					}
				}
			}
					FreeConsole();
			return 1;

		case IDC_BUTTON6://�d�w���ȁE�L�W
			//a = GetDlgItemInt(hwndL, IDC_EDIT3, NULL, TRUE);
			//VCS_MoveToPosition(keyHandle, NodeId, a * -256, 1, 1, &errorCode);
			//SetDlgItemInt(hwndL, IDC_EDIT9, a, TRUE);

			AllocConsole();//�R���\�[���\��
			freopen("CONOUT$", "w", stdout);//�W���o�͂��R���\�[���ɂ���
			freopen("CONIN$", "r", stdin);//�W�����͂��R���\�[���ɂ���

			//int64_t lastFrameID = 0; //The last frame received

			OpenConnection();
			while (!IsConnected)
				millisleep(100); //wait a bit to let the connection complete(�ڑ������܂ő҂�)

			printf("Connected.");
			//LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();
			//if (deviceProps)
				//printf("Using device %s.\n", deviceProps->serial);

			for (;;) {
				LEAP_TRACKING_EVENT* frame = GetFrame();
				if (frame && (frame->tracking_frame_id > lastFrameID)) {
					lastFrameID = frame->tracking_frame_id;
					printf("Frame %lli with %i hands.\n", (long long int)frame->tracking_frame_id, frame->nHands);
					for (uint32_t h = 0; h < frame->nHands; h++) {
						LEAP_HAND* hand = &frame->pHands[h];
						/* printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
									 hand->id,
									 (hand->type == eLeapHandType_Left ? "left" : "right"),
									 hand->palm.position.x,
									 hand->palm.position.y,
									 hand->palm.position.z);*/

						
						LEAP_DIGIT* finger0 = &frame->pHands[h].thumb;
						
						double f = finger0->distal.prev_joint.x;//�e�w�̐�[�ʒu
						double e = hand->palm.position.x;//��̂Ђ�̈ʒu
						double j = 0;
						
						printf("thumb distal position is %f\n", finger0->distal.prev_joint.x);//�e�w�̐�[�̍��̈ʒu
						
						printf("palm position is %f\n", hand->palm.position.x);

						if ((e < 0 && f < 0) && e > f) {//�w����̂Ђ�̈ʒu���}�C�i�X����̂Ђ�̈ʒu�̕����[���ɋ߂���
							j = (-f) - (-e);
							printf("thumb-palm distance is %f\n", j);
							if (j >= 0 && j < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����
							
								VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W                                                                                                                                           
								
							}
							else if (j >= 72) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, 96 * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, 137 * -256, 1, 1, &errorCode);//�d�w�L�W   
							}
							else if (j >= 1 && j < 72) {
								int i = j / 0.72;
								printf("%d%%\n", 100 - i);
								int l = 100 - i;
								VCS_MoveToPosition(keyHandle, NodeId + 4, (96 + (0.27 * l)) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (137 - (0.12 * l)) * -256, 1, 1, &errorCode);//�d�w�L�W 
							}
						}

						else if ((e < 0 && f < 0) && e < f) {//�w����̂Ђ�̈ʒu���}�C�i�X���w�̈ʒu�̕����[���ɋ߂���
							j = (-e) - (-f);
							printf("thumb-palm distance is %f\n", j);
							printf("100%%\n");
							VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

							VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
						}

						else if (e < f) {//��̂Ђ�̈ʒu���w�̈ʒu��菬������
							j = e - (-f);
							printf("thumb-palm distance is %f\n", j);
							printf("100%%\n");
							VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

							VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
						}
						else if (e > 0 && f < 0) {//�w�ʒu���}�C�i�X�Ŏ�̂Ђ�ʒu���v���X�̎�
							j = (-f) + e;
							printf("thumb-palm distance is %f\n", j);
							if (e < f) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
							}
							else if (j >= 0 && j < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
							}
							else if (j >= 72) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, 96 * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, 137 * -256, 1, 1, &errorCode);//�d�w�L�W 
							}
							else if (j >= 1 && j < 72) {
								int i = j / 0.72;
								printf("%d%%\n", 100 - i);
								int l = 100 - i;
								VCS_MoveToPosition(keyHandle, NodeId + 4, (96 + (0.27 * l)) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (137 - (0.12 * l)) * -256, 1, 1, &errorCode);//�d�w�L�W 
							}
						}
						else {
							j = -(f - e);
							printf("thumb-palm distance is %f\n", j);
							if (e < f) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
							}
							else if (j >= 0 && j < 1) {
								printf("100%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, (123 - j) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (125 - j) * -256, 1, 1, &errorCode);//�d�w�L�W    
							}
							else if (j >= 72) {
								printf("0%%\n");
								VCS_MoveToPosition(keyHandle, NodeId + 4, 96 * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, 137 * -256, 1, 1, &errorCode);//�d�w�L�W 
							}
							else if (j >= 1 && j < 72) {
								int i = j / 0.72;
								printf("%d%%\n", 100 - i);
								int l = 100 - i;
								VCS_MoveToPosition(keyHandle, NodeId + 4, (96 + (0.27 * l)) * -256, 1, 1, &errorCode);//�d�w����

								VCS_MoveToPosition(keyHandle, NodeId + 7, (137 - (0.12 * l)) * -256, 1, 1, &errorCode);//�d�w�L�W 
							}
						}
					}
				}
			} //ctrl-c to exit

			FreeConsole();

			return 1;
			
		/*case IDC_BUTTON8://�d�w�L�W
			a = GetDlgItemInt(hwndL, IDC_EDIT4, NULL, TRUE);
			VCS_MoveToPosition(keyHandle, NodeId, a * -256, 1, 1, &errorCode);
			SetDlgItemInt(hwndL, IDC_EDIT10, a, TRUE);
			return 1;
			*/
		/*case IDC_BUTTON11://�w���L�W
			a = GetDlgItemInt(hwndL, IDC_EDIT5, NULL, TRUE);
			VCS_MoveToPosition(keyHandle, NodeId, a * -256, 1, 1, &errorCode);
			SetDlgItemInt(hwndL, IDC_EDIT11, a, TRUE);
			return 1;
			*/
		/*case IDC_BUTTON10://�R�w�L�W
			a = GetDlgItemInt(hwndL, IDC_EDIT6, NULL, TRUE);
			VCS_MoveToPosition(keyHandle, NodeId, a * -256, 1, 1, &errorCode);
			SetDlgItemInt(hwndL, IDC_EDIT12, a, TRUE);
			return 1;
			*/
		}


	/*case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 1:
			SetDlgItemInt(hwndL, IDC_EDIT7, c, TRUE);
			return 1;
		}
		*/
	case WM_CLOSE:		// �~�{�^�����N���b�N�����Ƃ��̏��� 
		SetTimer(start, 1, 100, NULL);
		KillTimer(hwnd, 1);
		DestroyWindow(hwnd); 		//�_�C�A���O��j�� 
		return 1;
	}
		return 0;
	}



}


