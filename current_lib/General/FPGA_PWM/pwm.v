module pwm
(
	input wire refClock,		///<ref for in clk 800KHz, t = 1250uS
	input wire enDC,
	input wire writePeriod,
	input wire [15:0] data,
	output reg outPwm,
	output wire outEventCnt

);

reg [15:0] period = 124;
reg [15:0] periodCnt = 0;

reg [15:0] duty = 1000;
reg [15:0] dutyCnt = 0;

reg [1:0] sh_writePeriod;




///*************************************************************************
///shift reg for period strob
always @(posedge refClock) begin

sh_writePeriod <= { sh_writePeriod[0], writePeriod };

end

wire strobWritePeriod = (sh_writePeriod == 2'b01);


///*************************************************************************
///period counter 
always @(posedge refClock) begin

	if(strobWritePeriod && enDC) begin 
		period <= data; 
		periodCnt <= 0;  
		end
	else if(periodCnt == period) begin 
				periodCnt <= 0; 
				end
			else begin 
				periodCnt <= periodCnt + 1'b1; 
				end

end

wire strobEventRSTPeriodCnt = (periodCnt == 0);


///*************************************************************************
///duty counter 
always @(posedge refClock) begin

	if(dutyCnt == 1000) begin 
		dutyCnt <= 0; 
		end
	else begin 
		dutyCnt <= dutyCnt + 1'b1; 
		end

end

wire strobEventDutyCnt = (periodCnt == 0);

///*************************************************************************
///duty counter 
always @(posedge refClock) begin

	if(strobEventRSTPeriodCnt && !enDC) begin duty <= data; end
	
end




///*************************************************************************
///
always @(posedge refClock) begin

	if(strobEventRSTPeriodCnt) begin 
		outPwm <= 1'b1; 
		end	
	else if(periodCnt == duty) begin 
				outPwm <= 1'b0; 
				end

end

assign outEventCnt = (periodCnt == 0);

endmodule