classdef PlateLoader < hgsetget
    %PLATELOADER Controls the Beckman Coulter Plate Loader Robot
    %   Performs the basic actions to control the plate loader

    % TODO: If anybody wants to help my comments in this file certainly be
    % better. :)  Thanks.  Just fix up the Google Doc.
    
    properties
        serialRobot
        xAxisPosition
        isZAxisExtended
        isGripperClosed
        isPlatePresent
    end
    properties (Constant = true)
        defaultTimeTable = [0 60 20 30 0
            0 0 30 30 0
            0 30 0 30 0
            0 30 30 0 0
            0 30 20 60 0];
    end
    
    methods
        function obj = PlateLoader(portNumber)
            % Construct a PlateLoader Object
            
            % Close any open serial connections
            open_ports=instrfind('Type','serial','Status','open');
            if ~isempty(open_ports)
                 fclose(open_ports);
            end
            portStr = sprintf('COM%d',portNumber);
            obj.serialRobot = serial(portStr,'BaudRate',19200,'Terminator',10,'Timeout',1);
            response = robotOpenSeq(obj);
            % Had to print the response since a construct cannot return mulitple items
            fprintf(response);  
            obj.xAxisPosition = 3;
            obj.isZAxisExtended = false;
            obj.isGripperClosed = true;
            % TODO: When turned on there might be a plate present
            %   Can someone add code to get Plate status
            %   Maybe use the GRIPPER_STATUS command and ready string reply
            obj.isPlatePresent = false;
        end
        function response = reset(obj)
            % Reset robot
            fprintf(obj.serialRobot,'RESET');
            obj.xAxisPosition = 3;
            obj.isZAxisExtended = false;
            obj.isGripperClosed = true;
            response = getResponse(obj);
        end
        function response = x(obj,pos)
            % Moves the x-axis to position, passes the reply back to caller
            if (pos <1 || pos>5)
                fprintf('Illegal position');
                return
            end
            xCommand = sprintf('X-AXIS %d',pos);
            fprintf(obj.serialRobot,xCommand);
            if(obj.xAxisPosition ~= pos)
                obj.isZAxisExtended = false;
            end
            obj.xAxisPosition = pos;
            response = getResponse(obj);
        end
        function response = extend(obj)
            % Extends the Z-Axis, passes the reply back to caller
            fprintf(obj.serialRobot,'Z-AXIS EXTEND');
            response = getResponse(obj);
            if(strcmp(response(1:5),'ERROR'))
                obj.isZAxisExtended = false;
            else
                obj.isZAxisExtended = true;
            end
        end
        function response = retract(obj)
            % Retracts the Z-Axis, passes the reply back to caller
            fprintf(obj.serialRobot,'Z-AXIS RETRACT');
            obj.isZAxisExtended = false;
            response = getResponse(obj);
        end
        function response = close(obj)
            % Close Gripper, passes the reply back to caller
            fprintf(obj.serialRobot,'GRIPPER CLOSE');
            obj.isGripperClosed = true;
            response = getResponse(obj);
            if( strcmp(response(end-8:end-2),'NOPLATE'))
                obj.isPlatePresent = false;
            else
                obj.isPlatePresent = true;
            end
        end
        function response = open(obj)
            % Open Gripper, passes the reply back to caller
            fprintf(obj.serialRobot,'GRIPPER OPEN');
            obj.isGripperClosed = false;
            obj.isPlatePresent = false;
            response = getResponse(obj);
        end
        function response = movePlate(obj, startPos, endPos)
            % movePlate(startPos, endPos)- Passes two MATLAB numbers for the
            % start and end position of the plate, tries to move the plate to
            % that position, and passes the reply back to caller
            if (startPos <1 || startPos>5 || endPos <1 || endPos>5)
                fprintf('Illegal position');
                return
            end
            moveCommand = sprintf('MOVE %d %d',startPos,endPos);
            fprintf(obj.serialRobot,moveCommand);

            response = getResponse(obj);
            if( strcmp(response(1:5),'ERROR'))
                obj.xAxisPosition = startPos;
                obj.isZAxisExtended = false;
                obj.isGripperClosed = false;
                obj.isPlatePresent = false;
            else
                obj.xAxisPosition = 3;
                obj.isZAxisExtended = false;
                obj.isGripperClosed = true;
                obj.isPlatePresent = false;
            end
            
        end
        function response = setTimeValues(obj,timeDelays)
            % setTimeValues(timeDelays) - Passes a matrix with 5 rows (froms)
            % and 5 columns (tos) to set all the time delay value
            if (size(timeDelays) ~= [5 5])
                fprintf('Need a 5 by 5 matrix of time delays');
                return
            end
            for i = 1:5
                for j = 2:4
                    if(i ~= j)
                        timeCommand = sprintf('SET_DELAY %d %d %d', i,j,timeDelays(i,j));
                        fprintf(obj.serialRobot,timeCommand);
                        response = getResponse(obj);
                        fprintf(response);
                    end
                end
            end
        end
        function response = resetDefaultTimes(obj)
            % Resets the default time delay table values
            response = obj.setTimeValues(obj.defaultTimeTable);
        end
        function response = getStatus(obj)
            % Since we are keeping the status as instance fields we can just
            % get the properties of the class, this is a useful double check
            fprintf(obj.serialRobot,'LOADER_STATUS');
            response = getResponse(obj);
            % TODO: Make the values update if different
            %  Can someone make the call to LOADED_STATUS also update
            %  properties, just in case somehow it gets off
        end
        
        % Other to todo's if someone wants to.  Implement the additional
        %  weird commands: STOP_CYLINDER, VERSION, 
        %  X-AXIS_STATUS, Z-AXIS_STATUS, GRIPPER_STATUS
        
        function [xPos,zAxis,grip,plate] = getProperties(obj)
            % Returns the status properties of the robot (for GUI display)
            xPos = obj.xAxisPosition;
            zAxis = obj.isZAxisExtended;
            grip = obj.isGripperClosed;
            plate = obj.isPlatePresent;
        end
        function response = shutdown(obj)
            % Close serial object
            fclose(obj.serialRobot);
            response = 'Disconnected';
        end
        function disp(obj)
            % Overrides the display when seeing robot status
            % Note: if you need to see the field names use
            %    get(_objectName_)
            fprintf('  X-AXIS %d, ',obj.xAxisPosition);
            if (obj.isZAxisExtended)
                fprintf('EXTENDED, ');
            else
                fprintf('RETRACTED, ');
            end
            if (obj.isGripperClosed)
                if( obj.isPlatePresent )
                    fprintf('CLOSED, PLATE');
                else
                    fprintf('CLOSED, NOPLATE');
                end
            else
                fprintf('OPEN');
            end
            fprintf('\n');
        end
    end
end

function response = robotOpenSeq(obj)
% Open the serial port for PlateLoader and RESET
fprintf('Connect to robot...');
fprintf('Status is %s\n',obj.serialRobot.Status);
fprintf('Opening...');
fopen(obj.serialRobot)
fprintf('Status is %s\n',obj.serialRobot.Status);
fprintf(obj.serialRobot,'INITIALIZE');
response = getResponse(obj);
end



function response = getResponse(obj)
% Get the response from the serial robot port
warning off all
response = char(fread(obj.serialRobot,1));
while((isempty(response))||(~strcmp(response(end),char(get(obj.serialRobot,'Terminator')))))
    response = [response,char(fread(obj.serialRobot,1))];
end
warning on all
end
