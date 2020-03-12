module connection
(
	input wire refClock,
	output wire pllReady,
	
	input wire enDCWrite,
	input wire writePeriod,
	input wire [5:0] data,
	output wire eventRelodeDuty,
	output wire pwmOut
	



);

wire pll100MHz;



///*************************************************************************
///pll connection
pll_100MHz pll_100MHz_inst0(.inclk0(refClock), .locked(pllReady), .c0(pll100MHz));



///*************************************************************************
///pwm connection
pwm pwm_inst0(.refClock(pll100MHz), .enDC(enDCWrite), .writePeriod(writePeriod), .data(data), .outEventCnt(eventRelodeDuty), .outPwm(pwmOut));


endmodule