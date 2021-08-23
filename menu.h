
extern struct Panel *RawData;
int init_RawData();
int update_RawData(char *indata);

extern struct Panel *FlagData;
int init_FlagData();
int update_FlagData(struct ECM_Regs *eregs);

extern struct Panel *SensorData;
int init_SensorData();
int update_SensorData(struct ECM_Regs *eregs);

extern struct Panel *ErrorData;
int init_ErrorData();
int update_ErrorData(struct ECM_Regs *eregs);
