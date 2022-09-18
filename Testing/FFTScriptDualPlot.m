% code used from https://www.mathworks.com/matlabcentral/answers/308231-fft-from-csv-data-file

%FIGURE 1
filename = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\combined v2\Higher Sampled - subset 2,2409.txt";
X = readmatrix(filename);
Fs = 80000;                                       % Sampling frequency
T = 1/Fs;                                           % Sampling period
L = 35;                                           % Length of signal
t = [0.093999997 0.123999998 0.134000003 0.137999997 0.145999998 0.150000006 0.162 0.165999994 0.175999999 0.185000002 0.188999996 0.206 0.214000002 0.216999993 0.223000005 0.241999999 0.246000007 0.256000012 0.263999999 0.26699999 0.273999989 0.291999996 0.296000004 0.305999994 0.31400001 0.317999989 0.326000005 0.335000008 0.338 0.345999986 0.349999994 0.372000009 0.375999987 0.39199999];                                      % Time vector

Fn = Fs/2;                                          % Nyquist Frequency
FX = fft(X)/L;                                      % Fourier Transform
Fv = linspace(0, 1, fix(L/2)+1)*Fn;                 % Frequency Vector
Iv = 1:length(Fv);                                  % Index Vector

tmp1 = X(:,1);
tmp2 = X(:,2);
tmp3 = X(:,3);
Acc = [tmp1, tmp2, tmp3];
AccCorr = [(tmp1-mean(tmp1)), (tmp2 - mean(tmp2)), (tmp3-mean(tmp3))];

tmp4 = X(:,4);
tmp5 = X(:,5);
tmp6 = X(:,6); 
Gyro = [tmp4, tmp5, tmp6];
%tmpGyro = [tmp4, tmp5, tmp6];
%Gyro = smoothdata(tmpGyro, "gaussian", 50);
GyroCorr = [(tmp4-mean(tmp4)), (tmp5 - mean(tmp5)), (tmp6-mean(tmp6))];

Temp = X(:,7);
TempCorr = Temp - mean(Temp);

AllCorr = X - mean(X);

Fn = Fs/2;                                          % Nyquist Frequency
FAcc = fft(Acc)/L;                                      % Fourier Transform
FGyro = fft(Gyro)/L;
FTemp = fft(Temp)/L;
FAll = fft(X)/L;
Fv = linspace(0, 1, fix(L/2)+1)*Fn;                 % Frequency Vector
Iv = 1:length(Fv);                                  % Index Vector

%FIGURE 2
filename2 = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\combined v2\Higher Sampled - subset 2,2409 decrypted.txt";
X2 = readmatrix(filename);

tmp12 = X2(:,1);
tmp22 = X2(:,2);
tmp32 = X2(:,3);
Acc2 = [tmp12, tmp22, tmp32];
AccCorr2 = [(tmp12-mean(tmp12)), (tmp22 - mean(tmp22)), (tmp32-mean(tmp32))];

tmp42 = X2(:,4);
tmp52 = X2(:,5);
tmp62 = X2(:,6); 
Gyro2 = [tmp42, tmp52, tmp62];
%tmpGyro2 = [tmp42, tmp52, tmp62];
%Gyro2 = smoothdata(tmpGyro2, "gaussian", 50);
GyroCorr2 = [(tmp42-mean(tmp42)), (tmp52 - mean(tmp52)), (tmp62-mean(tmp62))];

Temp2 = X2(:,7);
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


