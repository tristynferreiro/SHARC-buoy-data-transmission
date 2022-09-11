inpFile = "C:\Users\9510 i9\Desktop\Encryption Testing S1\chachaencTest2\ChaCha20Poly1305V2\dtest.csv";
outFile = "C:\Users\9510 i9\Desktop\Encryption Testing S1\chachaencTest2\compression\new.csv";

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