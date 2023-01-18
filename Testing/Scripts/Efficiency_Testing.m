%PLOT EFFICIENCY GRAPHS
characters = [0,36, 179, 360, 545, 730];
comp = [0,6,31,63,97,135];
enc = [0,0,1,3,4,5];

plot(characters, comp);
hold on
plot(characters, enc);
hold on
plot(characters, comp+enc)