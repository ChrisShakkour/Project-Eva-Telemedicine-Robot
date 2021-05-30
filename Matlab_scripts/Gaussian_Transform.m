%{
this script was used to calibrate and explore the different types of
transforms used to map Robot Speed and angle of manuover to right and left
wheels speed on  adifferential drive robot
%}
%%
close all;
clear all;
clc;
%%
%Constants and difenetions
N=180;    %Num of angles
res=0.1;  %x-Axis resolution
b=90;     %The Expectance of a gaussian function(symetry point)
l=22;
speed=50; %Drive-train speed
% Gaussian Function 1
a1=3000.0;
c1=4.0;
% Gaussian function 2
a2=100.0;
c2=20.0;
% Gaussian function 3
a3=30.0;
c3=40.0;
% Gaussian function 4
a4=32.0;
c4=1000.0;
%%
x = 0:res:N;
% calculating the Transform
fx1 = a1*exp(-power(x-b,2)/(2*power(c1,2)));
fx2 = a2*exp(-power(x-b,2)/(2*power(c2,2)));
fx3 = a3*exp(-power(x-b,2)/(2*power(c3,2)));
fx4 = a4*exp(-power(x-b,2)/(2*power(c4,2)));
Hx = fx1+fx2+fx3+fx4;
% Drivetrain control functions
Rad = (l./Hx);
RWS = speed*(1-Rad);    %Right Wheel Speed 
LWS = speed*(1+Rad);    %Left Wheel Speed
%%
plot(x,fx1);
title('Gaussian series plots')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
hold on;
plot(x, fx2);
plot(x, fx3);
plot(x, fx4);
plot(x, Hx);
ylim([-10 110])
legend({'fx1','fx2','fx3','fx4','Hx'},'Location','northeast')
hold off;
%%
close all;
plot(x,RWS);
title('Left Right wheels speed')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
hold on;
plot(x,LWS);
legend({'RWS','LWS'},'Location','northeast')
%%
close all;
plot(x,speed*Rad);
title('Differential speed')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
hold on;
legend({'Differential Speed'},'Location','northeast')
%%
close all;
figure;
subplot(3,1,1);
plot(x,fx1);
title('Gaussian series plots')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
hold on;
plot(x, fx2);
plot(x, fx3);
plot(x, fx4);
plot(x, Hx);
legend({'fx1','fx2','fx3','fx4','Hx'},'Location','northeast')
hold off;

subplot(3,1,3);
plot(x,RWS);
title('Left Right wheels speed')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
hold on;
plot(x,LWS);
legend({'RWS','LWS'},'Location','northeast')
hold off;

subplot(3,1,2);
plot(x,speed*Rad);
title('Differential speed')
xlabel('Angle [deg]');
ylabel('magnitute');
grid on;
legend({'Differential Speed'},'Location','northeast')

