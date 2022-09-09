% code used from https://www.mathworks.com/matlabcentral/answers/308231-fft-from-csv-data-file

filename = 'C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Data\cleaned - Turntable Example Data 2.csv';
X = readmatrix(filename);
t = X(:,1);

tmp1 = X(:,3);
tmp2 = X(:,4);
tmp3 = X(:,5);
%tmpAcc = [tmp1, tmp2, tmp3];
%Acc = smoothdata(tmpAcc, "gaussian", 50);
Acc = [tmp1, tmp2, tmp3];

tmp4 = X(:,6);
tmp5 = X(:,7);
tmp6 = X(:,8); 
Gyro = [tmp4, tmp5, tmp6];

Temp = X(:,9);

figure(1)
plot(t, Acc)
grid
title('Accelerometer Data')
xlabel('Time')
ylabel('Amplitude')
saveas(1, "accTDTT2.png");

figure(2)
plot(t, Gyro)
grid
title('Gyroscope Data')
xlabel('Time')
ylabel('Amplitude')
saveas(2, "gyroTDTT2.png");

figure(3)
plot(t, Temp)
grid
title('Temperature Data')
xlabel('Time')
ylabel('Amplitude')
saveas(3, "tempTTT2.png");

% figure(4)
% plot(t,X)
% grid
% title('All Data')
% xlabel('Time')
% ylabel('Amplitude')
% saveas(4, "combinedTD.png");
