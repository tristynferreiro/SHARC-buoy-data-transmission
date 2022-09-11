% code used from https://www.mathworks.com/matlabcentral/answers/308231-fft-from-csv-data-file

%FIGURE 1
filename = '~/Desktop/Higher sampled turntable Example Data.csv';
X = readmatrix(filename);
Time = X(:,1);

tmp1 = X(:,3);
tmp2 = X(:,4);
tmp3 = X(:,5);
Acc = [tmp1, tmp2, tmp3];
AccCorr = [(tmp1-mean(tmp1)), (tmp2 - mean(tmp2)), (tmp3-mean(tmp3))];

tmp4 = X(:,6);
tmp5 = X(:,7);
tmp6 = X(:,8); 
Gyro = [tmp4, tmp5, tmp6];
%tmpGyro = [tmp4, tmp5, tmp6];
%Gyro = smoothdata(tmpGyro, "gaussian", 50);
GyroCorr = [(tmp4-mean(tmp4)), (tmp5 - mean(tmp5)), (tmp6-mean(tmp6))];

Temp = X(:,9);
TempCorr = Temp - mean(Temp);

AllCorr = X - mean(X);


Fs = 1000000;                                       % Sampling frequency
T = 1/Fs;                                           % Sampling period
L = numel(Time);                                           % Length of signal
t = Time;                                   % Time vector

Fn = Fs/2;                                          % Nyquist Frequency
FAcc = fft(Acc)/L;                                      % Fourier Transform
FGyro = fft(Gyro)/L;
FTemp = fft(Temp)/L;
FAll = fft(X)/L;
Fv = linspace(0, 1, fix(L/2)+1)*Fn;                 % Frequency Vector
Iv = 1:length(Fv);                                  % Index Vector

%FIGURE 2
filename2 = '~/Desktop/decompressed.csv';
X2 = readmatrix(filename);
Time = X2(:,1);

tmp12 = X2(:,3);
tmp22 = X2(:,4);
tmp32 = X2(:,5);
Acc2 = [tmp12, tmp22, tmp32];
AccCorr2 = [(tmp12-mean(tmp12)), (tmp22 - mean(tmp22)), (tmp32-mean(tmp32))];

tmp42 = X2(:,6);
tmp52 = X2(:,7);
tmp62 = X2(:,8); 
Gyro2 = [tmp42, tmp52, tmp62];
%tmpGyro2 = [tmp42, tmp52, tmp62];
%Gyro2 = smoothdata(tmpGyro2, "gaussian", 50);
GyroCorr2 = [(tmp42-mean(tmp42)), (tmp52 - mean(tmp52)), (tmp62-mean(tmp62))];

Temp2 = X2(:,9);
TempCorr2 = Temp2 - mean(Temp2);

AllCorr2 = X2 - mean(X2);

Fn = Fs/2;                                          % Nyquist Frequency
FAcc2 = fft(Acc2)/L;                                      % Fourier Transform
FGyro2 = fft(Gyro2)/L;
FTemp2 = fft(Temp2)/L;
FAll2 = fft(X2)/L;
Fv = linspace(0, 1, fix(L/2)+1)*Fn;                 % Frequency Vector
Iv = 1:length(Fv);                                  % Index Vector

%FIGURE1
FAccdcoc = fft(AccCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FGyrodcoc = fft(GyroCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FTempdcoc = fft(TempCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FAlldcoc = fft(AllCorr)/L;                          % Fourier Transform (D-C Offset Corrected)

%FIGURE2
FAccdcoc2 = fft(AccCorr2)/L;                          % Fourier Transform (D-C Offset Corrected)
FGyrodcoc2 = fft(GyroCorr2)/L;                          % Fourier Transform (D-C Offset Corrected)
FTempdcoc2 = fft(TempCorr2)/L;                          % Fourier Transform (D-C Offset Corrected)
FAlldcoc2 = fft(AllCorr2)/L;

figure(5)
plot(Fv, lowpass(abs(FAccdcoc(Iv))*2,0.1))
grid
title('Fourier Transform Of D-C Offset Corrected Accelerometer Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
hold on
plot(Fv, lowpass(abs(FAccdcoc2(Iv))*2,0.1))
saveas(5, "accCorrFFT.png");


figure(6)
plot(Fv, lowpass(abs(FGyrodcoc(Iv))*2,0.1))
grid
title('Fourier Transform Of D-C Offset Corrected Gyroscope Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
hold on
plot(Fv, lowpass(abs(FGyrodcoc2(Iv))*2,0.1))
saveas(6, "GyroCorrFFT.png");

figure(7)
plot(Fv, lowpass(abs(FTempdcoc(Iv))*2, 0.1))
grid
title('Fourier Transform Of D-C Offset Corrected Temperature Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
hold on
plot(Fv, lowpass(abs(FTempdcoc2(Iv))*2, 0.1))
saveas(7, "TempCorrFFT.png");

figure(8)
plot(Fv, lowpass(abs(FAlldcoc(Iv))*2, 0.1))
grid
title('Fourier Transform Of D-C Offset Corrected All Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
hold on
plot(Fv, lowpass(abs(FAlldcoc2(Iv))*2, 0.1))
saveas(8, "CombinedCorrFFT.png");


