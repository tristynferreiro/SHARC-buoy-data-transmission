inpFile = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\Data\Higher sampled turntable Example Data.csv";
outFile = "C:\Users\9510 i9\Desktop\SHARC_buoy_data_transmission\Testing\Data\Higher sampled turntable Example Data.csv";

in = readmatrix(inpFile);
out = readmatrix(outFile);

LIn = length(in);
LOut = length(out);

if (LIn ~= LOut)
    disp("not equal");
else 
    isEqual = mscohere(in, out);

    if (isEqual == 1)
        disp("equal");
    else
           disp("not equal"); 
    end
end