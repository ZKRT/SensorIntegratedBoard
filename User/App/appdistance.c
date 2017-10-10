/**
  ******************************************************************************
  * @file    appdistance.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   ���ϴ������ɼ�����
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "osusart.h"
#include "appdistance.h"
#ifdef USE_RADAR_FUN	
#include "radar.h"
#endif
#include "ToFSensorOP.h"
/* Private macro -------------------------------------------------------------*/
#define D_FILTER_CNY    1    //���˸���
/* Private variables ---------------------------------------------------------*/
uint8_t distance_utbuf[USART_RX_DATA_SIZE];  //���������ڽ��մ����棬4��̽��������һ�������棬4��̽����������һ�����ڵײ���ջ���
uint16_t dutbuf_len; //the actual received length of distance_utbuf
uint16_t distance10_v[4][D_FILTER_CNY]={0}; //4�������ϰ��������10���ϰ���������ݣ�������ƽ������
uint8_t distance10_v_index[4]={0}; //distance10_v[4][D_FILTER_CNY]�洢��������ÿ�������Ǵ���D_FILTER_CNY�������ƽ������
distance_info_t global_distance; //������Ϣ���������˴���
volatile uint8_t getdistance_flag=0;  //1-ÿ�����ڷ���һ�������ȡ����������
volatile uint8_t distance_recved_flag[4];  //�н��ܵ�������Ϣ�ı��
volatile uint8_t distance_recved_validedtimecnt[4]; //���յ�������Ϣ����Чʱ�����

const uint16_t distance_position[4]= {D_P_FRONT, D_P_LEFT, D_P_BACK, D_P_RIGHT};   //���봫������װλ�ö���---����ڻ������Զλ�ã�һ���ǻ�����
/* Private functions ---------------------------------------------------------*/
uint16_t vlaue_avg_filter(const uint16_t *value, uint16_t num);
uint8_t tof_radar_recv_handle(uint8_t *buf, uint16_t *buflen, uint8_t port, uint8_t distance_num);
void get_distance_cmdTask(void);
void no_recv_distance_Task(void);
/**
  * @brief  appdistance_init
  * @param  None
  * @retval None
  */
void appdistance_init(void)
{
	ToF_ContinuousStart();
#ifdef USE_RADAR_FUN		
	radar_Start();
#endif	
	t_ostmr_insertTask(get_distance_cmdTask, 30000, OSTMR_PERIODIC);  	
	t_ostmr_insertTask(no_recv_distance_Task, 100, OSTMR_PERIODIC);
}
/**
  * @brief  appdistance_prcs int main
  * @param  None
  * @retval None
  */
void appdistance_prcs(void)
{
	int i;
	char ret=0;
	//handle radar
	ret = tof_radar_recv_handle(distance_utbuf, &dutbuf_len, RADIO_UCOM, RADAR_NUM);
	if(ret)
	{
		ret =0;
		memset(distance_utbuf, 0, sizeof(distance_utbuf));
	}	
	//handle tof
	ret =tof_radar_recv_handle(distance_utbuf, &dutbuf_len, TOFLEFT_UCOM, TOFLEFT_NUM);
	if(ret)
	{
		ret =0;
		memset(distance_utbuf, 0, sizeof(distance_utbuf));
	}	
	ret =tof_radar_recv_handle(distance_utbuf, &dutbuf_len, TOFRIGHT_UCOM, TOFRIGHT_NUM);
	if(ret)
	{
		ret =0;
		memset(distance_utbuf, 0, sizeof(distance_utbuf));
	}		
	ret = tof_radar_recv_handle(distance_utbuf, &dutbuf_len, TOFBACK_UCOM, TOFBACK_NUM);
	if(ret)
	{
		ret =0;
		memset(distance_utbuf, 0, sizeof(distance_utbuf));
	}		
	
	//get distance cmd send in period of 30s
	if(getdistance_flag)
	{
		getdistance_flag = 0;
		ToF_ContinuousStart();
#ifdef USE_RADAR_FUN
		radar_easy_Start();
#endif
//		printf("distance sensor start\n");
	}
	//��ѯ����������Ч���
	for(i=0; i<4; i++)
	{
		if((!distance_recved_flag[i])&&(global_distance.distance_data[i])!=DISTANCE_NONE)  
		{
			global_distance.distance_data[i] = DISTANCE_NONE;
	//		printf("distace[%d]=%d\n", RADAR_NUM, global_distance.distance_data[RADAR_NUM]);  //zkrt_debug
		}
	}
}
/**
  * @brief  vlaue_avg_filter ȥ��������ƽ��ֵ

  * @param  None
  * @retval None
  */
uint16_t vlaue_avg_filter(const uint16_t *value, uint16_t num)
{
	int i;
	uint16_t max,maxnum,min,minnum,sum=0;
//	uint8_t maxvalided=0, minvalided=0;
	uint16_t pj = value[0];
	for(i=0; i<num; i++)
	{
		sum += value[i];
		if(i==0)
		{
			max = value[0];
			min = max;
			maxnum=1;
			minnum=1;
		}	
		else
		{
			if(value[i]>max)
			{		
				max =value[i];
				maxnum=1;
			}
			else if((value[i]==max))
			{
				maxnum++;
			}
			if(value[i]<min)
			{
				min = value[i];
				min=1;
			}
			else if((value[i]==min))
			{
				minnum++;
			}
		}
	}
//	if(maxnum>=num/2)
//		maxvalided =1;
//	if(minnum>=num/2)
//		minvalided =1;
//	if(num-maxnum-minnum==0)
//	{
//		maxvalided =1;
//		minvalided =1;
//	}
//	pj=(sum-max*maxnum*maxvalided-min*minnum*minvalided)/(num-maxnum*(~maxvalided)-minnum*(~minvalided));
	
	if(num <=3)
	{
		//��ȥ����ߣ����
		pj = sum/num;
	}	
	else
		pj = (sum-max-min)/(num-2);
	return pj;
}
/**
  * @brief  tof_radar_recv_handle
  * @param  None
  * @retval state
  */
uint8_t tof_radar_recv_handle(uint8_t *buf, uint16_t *buflen, uint8_t port, uint8_t distance_num)
{
	char ret =0;
	char avg_enable=0;
	uint16_t *this_distance = &distance10_v[distance_num][distance10_v_index[distance_num]];
		
//�������ݽ��գ���������
	if(t_osscomm_ReceiveMessage(buf, buflen, UsartInstance[port])==SCOMM_RET_OK)
	{
		ret =1;
#ifdef USE_RADAR_FUN		
		if(port == RADIO_UCOM)
		{
			if(radar_parse(buf, *buflen, &distance10_v[distance_num][distance10_v_index[distance_num]]))
			{
				avg_enable = 1;
			}
		}
		else
		{
			if(ToF_parse(buf, *buflen, &distance10_v[distance_num][distance10_v_index[distance_num]]))
			{
				avg_enable = 1;
			}	
		}
#else
		if(ToF_parse(buf, *buflen, &distance10_v[distance_num][distance10_v_index[distance_num]]))
		{
			avg_enable = 1;
		}
#endif
//�������ݵ����͹���
		if(avg_enable)
		{
			//���յ��������ݵı����λ
			distance_recved_flag[distance_num] = 1;              //���ձ��
			distance_recved_validedtimecnt[distance_num] = 4;    //400ms
			//��װλ�õ�������
		  if(*this_distance >=distance_position[distance_num])
			{
				if(*this_distance < DISTANCE_NONE)
				{
					//��λ�ù���
					*this_distance = (*this_distance) - distance_position[distance_num];
				}
        else
        {//>=DISTANCE_NONE����������ֵ����Ҫ����
				}					
			}
			else
			{//��������������С�ڰ�װλ��ʱ��������Ϊ0
				*this_distance = 0;
//				printf("distace[%d]=%d < install position!\n", distance_num, *this_distance);
			}
			
			//ƽ������
			distance10_v_index[distance_num]++;
			if(distance10_v_index[distance_num] ==D_FILTER_CNY)
			{
				distance10_v_index[distance_num] = 0;
				global_distance.distance_data[distance_num] = vlaue_avg_filter(&distance10_v[distance_num][0], D_FILTER_CNY);
//				printf("distace[%d]=%d\n", distance_num, global_distance.distance_data[distance_num]);  //zkrt_debug
			}
		}
	}
	return ret;
}
/**
  * @brief  get_distance_cmdTask
  * @param  None
  * @retval None
  */
void get_distance_cmdTask(void)
{
	if(!getdistance_flag)
	{
		getdistance_flag = 1;
	}	
}	
void no_recv_distance_Task(void)
{
	int i;
	for(i=0; i<4; i++)
	{
		if(distance_recved_validedtimecnt[i])
		{
			distance_recved_validedtimecnt[i]--;
			if(!distance_recved_validedtimecnt[i])
			{
				distance_recved_flag[i] = 0;
			}
		}
	}
}
/**
*@
*/
