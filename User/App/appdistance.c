/**
  ******************************************************************************
  * @file    appdistance.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   避障传感器采集处理
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
#define D_FILTER_CNY    1    //过滤个数
/* Private variables ---------------------------------------------------------*/
uint8_t distance_utbuf[USART_RX_DATA_SIZE];  //传感器串口接收处理缓存，4个探测器共用一个处理缓存，4个探测器各自有一个串口底层接收缓存
uint16_t dutbuf_len; //the actual received length of distance_utbuf
uint16_t distance10_v[4][D_FILTER_CNY]={0}; //4个方向障碍物的连续10个障碍物距离数据，用来作平均过滤
uint8_t distance10_v_index[4]={0}; //distance10_v[4][D_FILTER_CNY]存储的索引，每个方向都是存满D_FILTER_CNY个后进行平均过滤
distance_info_t global_distance; //距离信息（经过过滤处理）
volatile uint8_t getdistance_flag=0;  //1-每个周期发送一次命令获取传感器数据
volatile uint8_t distance_recved_flag[4];  //有接受到距离信息的标记
volatile uint8_t distance_recved_validedtimecnt[4]; //接收到距离信息的有效时间计数

const uint16_t distance_position[4]= {D_P_FRONT, D_P_LEFT, D_P_BACK, D_P_RIGHT};   //距离传感器安装位置定义---相对于机体的最远位置（一般是机浆）
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
	//查询距离数据有效标记
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
  * @brief  vlaue_avg_filter 去最高最低求平均值

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
		//不去掉最高，最低
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
		
//串口数据接收，解析处理
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
//距离数据调整和过滤
		if(avg_enable)
		{
			//接收到距离数据的标记置位
			distance_recved_flag[distance_num] = 1;              //接收标记
			distance_recved_validedtimecnt[distance_num] = 4;    //400ms
			//安装位置调整过滤
		  if(*this_distance >=distance_position[distance_num])
			{
				if(*this_distance < DISTANCE_NONE)
				{
					//做位置过滤
					*this_distance = (*this_distance) - distance_position[distance_num];
				}
        else
        {//>=DISTANCE_NONE等特殊数据值不需要过滤
				}					
			}
			else
			{//传感器距离数据小于安装位置时，距离置为0
				*this_distance = 0;
//				printf("distace[%d]=%d < install position!\n", distance_num, *this_distance);
			}
			
			//平均过滤
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
