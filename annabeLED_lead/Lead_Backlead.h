#define NUM_FIGURES 12

typedef struct Report_to_lead{
  uint16_t acc_value;
  uint8_t user_hsv[3];
  bool display_flag;
  uint8_t user_selection;
} Report_to_lead;

Report_to_lead report;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&report, incomingData, sizeof(Report_to_lead));
  figure.hue = report.user_hsv[0]*256;
  figure.sat = report.user_hsv[1]*2;
  figure.val = report.user_hsv[2]*2;

  if(report.user_selection >= NUM_FIGURES){
    figure.use_state = true;
  }
  else(figure.use_state = false);

}
