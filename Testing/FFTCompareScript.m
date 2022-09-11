% code used from https://www.mathworks.com/matlabcentral/answers/308231-fft-from-csv-data-file

inpFile = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\combined testing results\Lower sampled turntable Example Data 2.csv";
outFile = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\combined testing results\decompressed.csv";

in = readmatrix(inpFile);
Out = readmatrix(outFile);

TimeIn = in(:,1);
TimeOut= Out(:,1);


tmp1In = in(:,3);
tmp2In = in(:,4);
tmp3In = in(:,5);
AccIn = [tmp1In, tmp2In, tmp3In];
AccCorrIn = [(tmp1In-mean(tmp1In)), (tmp2In - mean(tmp2In)), (tmp3In-mean(tmp3In))];
tmp1Out = Out(:,3);
tmp2Out = Out(:,4);
tmp3Out = Out(:,5);
AccOut = [tmp1Out, tmp2Out, tmp3Out];
AccCorrOut = [(tmp1Out-mean(tmp1Out)), (tmp2Out - mean(tmp2Out)), (tmp3Out-mean(tmp3Out))];


tmp4In = in(:,6);
tmp5In = in(:,7);
tmp6In = in(:,8); 
GyroIn = [tmp4In, tmp5In, tmp6In];
GyroCorrIn = [(tmp4In-mean(tmp4In)), (tmp5In - mean(tmp5In)), (tmp6In-mean(tmp6In))];
tmp4Out = Out(:,6);
tmp5Out = Out(:,7);
tmp6Out = Out(:,8); 
GyroOut = [tmp4Out, tmp5Out, tmp6Out];
GyroCorrOut = [(tmp4Out-mean(tmp4Out)), (tmp5Out - mean(tmp5Out)), (tmp6Out-mean(tmp6Out))];

TempIn = in(:,9);
TempCorrIn = TempIn - mean(TempIn);
TempOut = Out(:,9);
TempCorrOut = TempOut - mean(TempOut);

Fs = 1000000;                                       % Sampling frequency
T = 1/Fs;                                           % Sampling period
LIn = numel(TimeIn);                                           % Length of signal
tIn = TimeIn;                                   % Time vector
LOut = numel(TimeOut);                                           % Length of signal
tOut = TimeOut;  

Fn = Fs/2;                                          % Nyquist Frequency
FAccIn = fft(AccIn)/LIn;                                      % Fourier Transform
FGyroIn = fft(GyroIn)/LIn;
FTempIn = fft(TempIn)/LIn;
FvIn = linspace(0, 1, fix(LIn/2)+1)*Fn;                 % Frequency Vector
IvIn = 1:length(FvIn);                                  % Index Vector

Fn = Fs/2;                                          % Nyquist Frequency
FAccOut = fft(AccOut)/LOut;                                      % Fourier Transform
FGyroOut = fft(GyroOut)/LOut;
FTempOut = fft(TempOut)/LOut;
FvOut = linspace(0, 1, fix(LOut/2)+1)*Fn;                 % Frequency Vector
IvOut = 1:length(FvOut);                                  % Index Vector


FAccdcocIn = fft(AccCorrIn)/LIn;                          % Fourier Transform (D-C Offset Corrected)
FGyrodcocIn = fft(GyroCorrIn)/LIn;                          % Fourier Transform (D-C Offset Corrected)
FTempdcocIn = fft(TempCorrIn)/LIn;                          % Fourier Transform (D-C Offset Corrected)
FAccdcocOut = fft(AccCorrOut)/LOut;                          % Fourier Transform (D-C Offset Corrected)
FGyrodcocOut = fft(GyroCorrOut)/LOut;                          % Fourier Transform (D-C Offset Corrected)
FTempdcocOut = fft(TempCorrOut)/LOut;                          % Fourier Transform (D-C Offset Corrected)
INPUTaccCorrFFT = lowpass(abs(FAccdcocIn(IvIn))*2,0.1);
INPUTGyroCorrFFT = lowpass(abs(FGyrodcocIn(IvIn))*2,0.1);
INPUTTempCorrFFT = lowpass(abs(FTempdcocIn(IvIn))*2, 0.1);

OUTPUTaccCorrFFT = lowpass(abs(FAccdcocOut(IvOut))*2,0.1);
OUTPUTGyroCorrFFT = lowpass(abs(FGyrodcocOut(IvOut))*2,0.1);
OUTPUTTempCorrFFT = lowpass(abs(FTempdcocOut(IvOut))*2, 0.1);

if (LIn ~= LOut)
    disp("not equal");
else 
    acc = mscohere(INPUTaccCorrFFT, OUTPUTaccCorrFFT);
    gyro = mscohere(INPUTGyroCorrFFT, OUTPUTGyroCorrFFT);
    temp = mscohere(INPUTTempCorrFFT, OUTPUTTempCorrFFT);

    if (acc == 1)
        if ( gyro == 1)
            if (temp == 1)
                disp("equal");
            end
        end
    else
           disp("not equal"); 
    end
end

