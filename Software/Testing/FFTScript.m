% code used from https://www.mathworks.com/matlabcentral/answers/308231-fft-from-csv-data-file

filename = 'C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Data\cleaned -  Turntable Example Data.csv';
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

figure(1)
plot(Fv, abs(FAcc(Iv))*2)
grid
title('Fourier Transform Of Accelerometer Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(1, "accFFT.png");

figure(2)
plot(Fv, abs(FGyro(Iv))*2)
grid
title('Fourier Transform Of Gyroscope Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(2, "gyroFFT.png");

figure(3)
plot(Fv, abs(FTemp(Iv))*2)
grid
title('Fourier Transform Of Temperature Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(2, "tempFFT.png");

figure(4)
plot(Fv, abs(FAll(Iv))*2)
grid
title('Fourier Transform Of All Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(2, "combinedFFT.png");

FAccdcoc = fft(AccCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FGyrodcoc = fft(GyroCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FTempdcoc = fft(TempCorr)/L;                          % Fourier Transform (D-C Offset Corrected)
FAlldcoc = fft(AllCorr)/L;                          % Fourier Transform (D-C Offset Corrected)

figure(5)
plot(Fv, abs(FAccdcoc(Iv))*2)
grid
title('Fourier Transform Of D-C Offset Corrected Accelerometer Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(2, "accCorrFFT.png");


figure(6)
plot(Fv, abs(FGyrodcoc(Iv))*2)
grid
title('Fourier Transform Of D-C Offset Corrected Gyroscope Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(4, "GyroCorrFFT.png");

figure(7)
plot(Fv, abs(FTempdcoc(Iv))*2)
grid
title('Fourier Transform Of D-C Offset Corrected Temperature Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(4, "TempCorrFFT.png");

figure(8)
plot(Fv, abs(FAlldcoc(Iv))*2)
grid
title('Fourier Transform Of D-C Offset Corrected All Data')
xlabel('Frequency (Hz)')
ylabel('Amplitude')
saveas(4, "CombinedCorrFFT.png");


