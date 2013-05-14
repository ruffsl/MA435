classdef PlateLoaderSim < hgsetget
    %PLATELOADERSIM Simulator class that uses no hardware
    %   Same functions as PlateLoader which would control the Beckman Coulter Plate Loader Robot
    %   Performs the basic actions to control the plate loader but doesn't
    %    send any serial commands out.  Only sends information to screen.

    properties
        xAxisPosition      % 1 2 3 4 or 5 storing X-AXIS location
        isZAxisExtended    % true = Z-AXIS Extended,  false = Z-AXIS Retracted
        isGripperClosed    % true = GRIPPER closed,   false = GRIPPER open
        isPlatePresent     % true = Plate in gripper, false = No plate
    end
    properties (Constant = true)
        % Default time delay values used in Beckman Coulter Plate Loader
        defaultTimeTable = [0 60 20 30 0
            0 0 30 30 0
            0 30 0 30 0
            0 30 30 0 0
            0 30 20 60 0];
    end

    methods
        function obj = PlateLoaderSim(unusedComPort)
            % Construct a PlateLoader Object
            fprintf('READY, SAGIAN PE Loader, ROM Ver. 1.1.6 12APR2001\n');
            obj.xAxisPosition = 3;
            obj.isZAxisExtended = false;
            obj.isGripperClosed = true;
            obj.isPlatePresent = false;
        end
        function response = reset(obj)
            % Reset robot
            fprintf('RESET\n'); %send to robot (console)
            obj.xAxisPosition = 3;
            obj.isZAxisExtended = false;
            obj.isGripperClosed = true;
            response = 'READY, SAGIAN PE Loader, ROM Ver. 1.1.6 12APR2001';
        end
        function response = x(obj,pos)
            % Moves the x-axis to position, passes the reply back to caller
            if (pos <1 || pos>5)
                fprintf('Illegal position');
                return
            end
            xCommand = sprintf('X-AXIS %d\n',pos);
            fprintf(xCommand); %send to robot (console)
            if(obj.xAxisPosition ~= pos)
                obj.isZAxisExtended = false;
            end
            obj.xAxisPosition = pos;
            response = 'READY';
        end
        function response = extend(obj)
            % Extends the Z-Axis, passes the reply back to caller
            fprintf('Z-AXIS EXTEND\n'); %send to robot (console)
            obj.isZAxisExtended = true;
            response = 'READY, EXTENDED';
        end
        function response = retract(obj)
            % Retracts the Z-Axis, passes the reply back to caller
            fprintf('Z-AXIS RETRACT\n'); %send to robot (console)
            obj.isZAxisExtended = false;
            response = 'READY, RETRACTED';
        end
        function response = close(obj)
            % Close Gripper, passes the reply back to caller
            fprintf('GRIPPER CLOSE\n'); %send to robot (console)
            obj.isGripperClosed = true;
            response = 'READY, CLOSED, NOPLATE';
%             response = 'READY, CLOSED, PLATE';
            if( strcmp(response(end-6:end),'NOPLATE'))
                obj.isPlatePresent = false;
            else
                obj.isPlatePresent = true;
            end
        end
        function response = open(obj)
            % Open Gripper, passes the reply back to caller
            fprintf('GRIPPER OPEN\n'); %send to robot (console)
            obj.isGripperClosed = false;
            obj.isPlatePresent = false;
            response = 'READY, OPEN';
        end
        function response = movePlate(obj, startPos, endPos)
            % movePlate(startPos, endPos)- Passes two MATLAB numbers for the
            % start and end position of the plate, tries to move the plate to
            % that position, and passes the reply back to caller
            if (startPos <1 || startPos>5 || endPos <1 || endPos>5)
                fprintf('Illegal position');
                return
            end
            moveCommand = sprintf('MOVE %d %d\n',startPos,endPos);
            fprintf(moveCommand); %send to robot (console)
            obj.xAxisPosition = 3;
            obj.isZAxisExtended = false;
            obj.isGripperClosed = true;
            obj.isPlatePresent = false;
            response = 'READY';
        end
        function response = setTimeValues(obj,timeDelays)
            % setTimeValues(timeDelays) - Passes a matrix with 5 rows (froms)
            % and 5 columns (tos) to set all the time delay value
            if (size(timeDelays) ~= [5 5])
                fprintf('Need a 5 by 5 matrix of time delays\n');
                return
            end
            for i = 1:5
                for j = 2:4
                    if(i ~= j)
                        timeCommand = sprintf('SET_DELAY %d %d %d\n', i,j,timeDelays(i,j));
                        fprintf(timeCommand); %send to robot (console)
                        response = sprintf('READY, From %d, To %d, Delay %d\n',i,j,timeDelays(i,j));
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
            
            % TODO: Make the values update if different
            fprintf('LOADER_STATUS\n'); %send to robot (console)
            
            response = sprintf('READY, POSITION %d',obj.xAxisPosition);
            if (obj.isZAxisExtended)
                response = strcat(response,', Z-AXIS EXTENDED');
            else
                response = strcat(response,', Z-AXIS RETRACTED');
            end
            if (obj.isGripperClosed)
                if( obj.isPlatePresent )
                    response = strcat(response,', GRIPPER CLOSED, PLATE');
                else
                    response = strcat(response,', GRIPPER CLOSED, NOPLATE');
                end
            else
                response = strcat(response,', GRIPPER OPEN');
            end
        end
        function [xPos,zAxis,grip,plate] = getProperties(obj)
            % Returns the status properties of the robot (for GUI display)
            xPos = obj.xAxisPosition;
            zAxis = obj.isZAxisExtended;
            grip = obj.isGripperClosed;
            plate = obj.isPlatePresent;
        end
        function response = shutdown(obj)
            % Close serial object
            response = 'Disconnected';
            % Could delete obj but no real need
        end
        function disp(obj)
            % Overrides the display when seeing robot status
            % Note: if you need to see the field names use:
            %    get(_objectName_)   for example "get(robot)"
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

