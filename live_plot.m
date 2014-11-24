%% Initialize program and USB port
if(exist('M2USB'))
  fclose(M2USB);
 else
%  fclose(instrfindall);
 delete(instrfindall);
end
clear all;
close all;

%% VARIABLES
mWii_Log = zeros(5,2);   % mWii data points logged here in format: [time ADC_Value]
mWii_x = zeros(1,8);
mWii_y = zeros(1,8);

%% SERIAL CONNECTION
M2USB = serial('/dev/cu.usbmodem411','Baudrate',9600);
% *** Check where your device is by opening terminal and entering the command:
% 'ls /dev/tty.usb' and tab-completing.

fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.

% Send initial packet to get first set of data from microcontroller
fwrite(M2USB,1);% Send a packet to the M2.
time = 0;       % Set the start time to 0.
tic;            % Start timer.

%%Set up Figure
figure(1);
clf;
hold on
title('mWii values');

%Send initial confirmation packet
fwrite(M2USB,1);            % Confirmation packet

%% Run program forever
try
    while 1

        %% Read in data and send confirmation packet
        m2_buffer = fgetl(M2USB);   % Load buffer
        fwrite(M2USB,1);            % Confirmation packet

        %% Parse microcontroller data
        [x1, remain] = strtok(m2_buffer);
        [y1, remain2] = strtok(remain);
%        [x2, remain3] = strtok(remain2);
%        [y2, remain4] = strtok(remain3);
%        [x3, remain5] = strtok(remain4);
%        [y3, remain6] = strtok(remain5);
%        [x4, remain7] = strtok(remain6);
%        [y4, remain8] = strtok(remain7);
%        [x, remain9] = strtok(remain8);
%        [y, remain10] = strtok(remain9);
        time = toc;            % Stamp the time the value was received

        % Remove the oldest entry
        mWii_x = [str2double(x1)]
        mWii_y = [str2double(y1)]
%        mWii_x = [str2double(x1) str2double(x2) str2double(x3) str2double(x4)]
%        mWii_y = [str2double(y1) str2double(y2) str2double(y3) str2double(y4)]

        %% Plotting
        clf;
        title('Sensor Values');

        hold on
        scatter(mWii_x,mWii_y, 'o');
        xlabel('X');
        ylabel('Y');

        xlim([0 1028]);
        ylim([0 768]);

        plot([0,0], [0,768]);
        plot([0,1028], [0,0]);
        plot([512,512], [0,768], ':r');
        plot([0,1028], [384,384], ':r');

        grid on;
        grid minor;

        pause(.00000001);

        hold off
    end
catch ME
    ME.stack
    %Close serial object
    fclose(M2USB);
end
