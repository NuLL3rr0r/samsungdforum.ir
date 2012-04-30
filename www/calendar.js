    /*var dayInvalid = new Array('10', '6', '29','2','17');
    var hourInvalid = new Array('10', '6', '29','2','17');*/
    function drawCalender(num,dayInvalid){

        document.getElementById('taghvimScript').innerHTML = '';
        document.getElementById('selectedDay').innerHTML = '-20';
        var taghvimString = '<table dir="rtl" cellspacing="1">'

        var daysWeek = new Array('شنبه', 'یک', 'دو','سه','چهار','پنج','جمعه','','');
        var mountName = new Array('آبان', 'آذر', 'دی','بهمن','اسفند');
        var numMounth = num - 8;
        var firstDayMonth = new Array(1,3,5,0,2);
        //================================================================================
        var headTr = '<tr style="width:375px;height:30px;">';
        taghvimString += headTr;
        for(var i=0;i<4;i++){
            var tdHead = '<td id="headTd'+(i+1)+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="width:40px;cursor:pointer;background:#eee;text-align:center;font:12px/30px tahoma;"></td>'
            if(i==1){
                tdHead = '<td id="headTd'+(i+1)+'" colSpan="5" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="width:40px;cursor:pointer;background:#eee;text-align:center;font:12px/30px tahoma;">'+mountName[numMounth]+'</td>'
            }
            if(i==0){
                tdHead = '<td id="headTd'+(i+1)+'" onclick="priviousMonth()" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="width:40px;cursor:pointer;background:#eee;text-align:center;font:12px/30px tahoma;">&lt;</td>'
            }

            if(i==2){
                tdHead = '<td id="headTd'+(i+1)+'" onclick="nextMonth()"  onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="width:40px;cursor:pointer;background:#eee;text-align:center;font:12px/30px tahoma;">&gt;</td>'
            }
            if(i==3){
                var tdHead = '<td id="headTd'+(i+1)+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="width:40px;cursor:pointer;background:#eee;text-align:center;font:12px/30px tahoma;">x</td>'
            }
            taghvimString+=tdHead;
        }
        var endtr = '</tr>';
        taghvimString +=endtr;
        
        //tb.appendChild(headTr);
        //================================================================================

        for(var i=0;i<7;i++){
            var trTaghvim = '<tr id="trTaghvim'+(i+1)+'" style="height:30px;">';
            taghvimString +=trTaghvim

            for(var j=0;j<7;j++){
                var dateNum = ((i-1)*7)+(j)-firstDayMonth[numMounth];
                var tdTrTaghvim = '<td id="td'+dateNum+'" style="font:12px/30px tahoma;text-align:center;background:#eee;color:#000;cursor:pointer;width:46px;"></td>';

                if(i==0){
                    if(j==6){
                        tdTrTaghvim = '<td colspan="2" id="td'+dateNum+'" onmouseover="tdOver(id,0)" onmouseout="tdOut(id,0)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;">'+daysWeek[j]+'</td>';
                    }else if(j==5){
                        tdTrTaghvim = '<td  id="td'+dateNum+'" onmouseover="tdOver(id,0)" onmouseout="tdOut(id,0)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;">'+daysWeek[j]+'</td>';
                    }else{
                        tdTrTaghvim = '<td id="td'+dateNum+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:#000;cursor:pointer;width:46px;">'+daysWeek[j]+'</td>';
                    }

                }else if(j==6){
                    if(((i-1)*7)+(j)>firstDayMonth[numMounth] && ((i-1)*7)+(j)-firstDayMonth[numMounth]<31){
                        tdTrTaghvim = '<td colspan="2" onmouseover="tdOver(id,0)" onmouseout="tdOut(id,0)" id="td'+dateNum+'" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;">'+dateNum+'</td>';
                    }else{
                        tdTrTaghvim = '<td colspan="2" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" id="td'+dateNum+'" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;"></td>';
                    }
                }else if(j==5 || j==3 || j==2 || j==1){
                    if(((i-1)*7)+(j)>firstDayMonth[numMounth] && ((i-1)*7)+(j)-firstDayMonth[numMounth]<31){
                        tdTrTaghvim = '<td id="td'+dateNum+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,0)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;">'+dateNum+'</td>';
                    }else{
                        tdTrTaghvim = '<td  onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" id="td'+dateNum+'" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;"></td>';
                    }
                }else{
                    if(!inArray(dateNum, dayInvalid)){
                        if(((i-1)*7)+(j)>firstDayMonth[numMounth] && ((i-1)*7)+(j)-firstDayMonth[numMounth]<31){
                            tdTrTaghvim = '<td id="td'+dateNum+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" onclick="tdClick(id)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:#000;cursor:pointer;width:46px;">'+dateNum+'</td>';
                        }else{
                            tdTrTaghvim = '<td id="td'+dateNum+'" onmouseover="tdOver(id,2)" onmouseout="tdOut(id,2)" style="font:12px/30px tahoma;text-align:center;background:#eee;color:red;cursor:pointer;width:46px;"></td>';
                        }

                    }else{
                        tdTrTaghvim = '<td id="td'+dateNum+'" onmouseover="tdOver(id,1)" onmouseout="tdOut(id,1)" style="font:12px/30px tahoma;text-align:center;background:red;color:#fff;cursor:pointer;width:46px;">'+dateNum+'</td>';
                    }
                }
                taghvimString +=tdTrTaghvim;
            }
            taghvimString +='</tr>';
        }

        //}
        taghvimString +='</table>'
        //================================================================================
        document.getElementById('taghvimScript').innerHTML = taghvimString
    }
    function tdOver(id,flag){
        var numDay = parseInt(id.substr(2,id.length));
        var selectedDay = parseInt(document.getElementById('selectedDay').innerHTML);
        if(selectedDay!= numDay){
        if(flag!=1){
            document.getElementById(id).style.background = '#6f6c6c';
            document.getElementById(id).style.color = '#fff';
        }else{
            document.getElementById(id).style.background = '#Dc143c';
            document.getElementById(id).style.color = '#fff';
        }
        }

    }

    function tdOut(id,flag){
        var numDay = parseInt(id.substr(2,id.length));
        var selectedDay = parseInt(document.getElementById('selectedDay').innerHTML);
        //alert(id);
        if(selectedDay!= numDay){
        if(flag==0){
            document.getElementById(id).style.color = 'red';
            document.getElementById(id).style.background = '#eee';
        }else if(flag==1){
            document.getElementById(id).style.background = 'red';
            document.getElementById(id).style.color = '#fff';
        }else if(flag==2){
            document.getElementById(id).style.background = '#eee';
            document.getElementById(id).style.color = '#000';
        }
        }
    }
    function nextMonth(){
        var numMounth =parseInt(document.getElementById('numMo').innerHTML);

        if(numMounth+1<5){
            numMounth++
            
            Wt.emit('AppointmentWidget', 'JSNextMonthClicked', numMounth + 8);
            
            //alert(numMounth);
            /*drawCalender(numMounth,dayInvalid);
            document.getElementById('numMo').innerHTML = numMounth;*/
            
        }
    }
    function priviousMonth(){
        var numMounth = parseInt(document.getElementById('numMo').innerHTML);
        if(numMounth-1>=0){
            numMounth--;
            
            Wt.emit('AppointmentWidget', 'JSPreviousMonthClicked', numMounth + 8);

            /*drawCalender(numMounth,dayInvalid);
            document.getElementById('numMo').innerHTML = numMounth;*/
        }
    }
    function tdClick(id){
        
        
        var numMounth = parseInt(document.getElementById('numMo').value);
        var numDay = parseInt(id.substr(2,id.length));

        var selectDay = parseInt(document.getElementById('selectedDay').innerHTML);

        document.getElementById('selectedDay').innerHTML = numDay;
        document.getElementById('td'+numDay).style.background = 'green';

        if(selectDay == -20){
        }else{
            tdOut('td'+selectDay,2);
        }
        /*if(selectedDay!=-20){
            tdOut('td'+selectedDay,2);
        }
        document.getElementById(id).style.background = '#808080';
        document.getElementById('selectedDay').innerHTML = numDay;
        */
        Wt.emit('AppointmentWidget', 'JSADaySelected', numDay);
        
        /*var numMounth = parseInt(document.getElementById('numMo').innerHTML);
        var numDay = parseInt(id.substr(2,id.length));
        alert('numMounth = '+ numMounth + 'numDay='+numDay);
        //drawDay(hourInvalid);
        preperHour();*/
    }


    


    function parser(forum,flag){
        var values = new Array() ;
        for (var i = 0; i < forum.length; i++) {
            var object = forum[i];
            for (property in object) {
                var value = object[property];
                //alert(property + "=" + value); // This alerts "id=1", "created=2010-03-19", etc..
                //alert(values);
                //if(i!=0){
                //  values += ',' + value  ;
                //}else{
                //values += value  ;
                //}
                values[i] = value;
            }
        }
        if(flag == 0){
            return value;
        }else{
            return values;
        }
    }



    function preperTaghvim(month, json){
        document.getElementById('numMo').innerHTML = month - 8;
        document.getElementById('selectedHour').innerHTML = -20;
        //var json = '{"Months":[{"ReservedDays":[{"Day1": 1},{"Day2": 5},{"Day3": 8},{"Day4": 15},{"Day4": 21}]}]}';
        obj = JSON.parse(json);
        var forum = obj.Months;
        var rs = parser(forum,0);
        var dayInvalid = parser(rs,1);
        drawCalender(month,dayInvalid);
    }

    //preperTaghvim();

    function drawHour(hourInvalid){
        document.getElementById('showHour').innerHTML = '';
        //var divShow = document.createElement('div');
        var showHourString = '';
        for(var i=0;i<12;i++){
            if(inArray((i+8), hourInvalid)){
                var divParent = '<div id="parent'+(i+8)+'" onmouseover="hourOver(id)" onmouseout="hourOut(id)" style="background:#474747;color:#fff;cursor:pointer;width: 100%;">';
            }else{
                var divParent = '<div id="parent'+(i+8)+'" onmouseover="hourOver(id)" onmouseout="hourOut(id)" onclick="hourClick(id)" style="background:#474747;color:#fff;cursor:pointer;width: 100%;">';
            }
            var divFrom = '<div '+(inArray((i+8), hourInvalid)?'':'onclick="hourClick(id)"')+'  id="fromHo'+(i+8)+'" style="width:30px;height:30px;float:left;border-top:1px solid #fff;font:12px/30px tahoma;">'+(i+8)+'</div>';

            var divLine = '<div '+(inArray((i+8), hourInvalid)?'':'onclick="hourClick(id)"')+' id="lineHo'+(i+8)+'" style="width:200px;height:30px;float:left;border-top:1px solid #fff;font:12px/30px tahoma;text-align:center;background:'+(inArray((i+8), hourInvalid)?'red':'')+'"></div>';

             var divUntil = '<div '+(inArray((i+8), hourInvalid)?'':'onclick="hourClick(id)"')+' id="untilH'+(i+8)+'" style="width:30px;height:30px;float:left;border-top:1px solid #fff;font:12px/30px tahoma;">'+((i+8)+1)+'</div>';

            var divClear = '<div style="clear:both"></div>';
            var endparentDiv = '</div>'
            showHourString += divParent;
            showHourString += divFrom;
            showHourString += divLine;
            showHourString += divUntil;
            showHourString += divClear;
            showHourString += endparentDiv;

            //            document.getElementById('showHour').innerHTML += showHourString;
            //            alert('message');
        }
        document.getElementById('showHour').innerHTML = showHourString;
        //        alert(showHourString);
    }

    function hourOver(id){
        var numHour = parseInt(id.substr(6,id.length));
        var selectHour = parseInt(document.getElementById('selectedHour').innerHTML);
        if(selectHour!= numHour){
            document.getElementById(id).style.background = '#eee';
            document.getElementById(id).style.color = '#000';

        }
    }
    function hourOut(id){
        var numHour = parseInt(id.substr(6,id.length));
        var selectHour = parseInt(document.getElementById('selectedHour').innerHTML);
        //alert(id);
        if(selectHour!= numHour){
            document.getElementById(id).style.background = '#474747';
            document.getElementById(id).style.color = '#fff';
        }
    }
    function hourClick(id){
    
        var numHour = parseInt(id.substr(6,id.length));
        var selectedHour = parseInt(document.getElementById('selectedHour').innerHTML);
        //alert(selectedHour);


        if(selectedHour == -30){
        
        }else{
            document.getElementById('parent'+selectedHour).style.background = '#474747';
            document.getElementById('parent'+selectedHour).style.color = '#fff';
        }
        document.getElementById('selectedHour').innerHTML = numHour;
        document.getElementById('parent'+numHour).style.background = 'green';
        Wt.emit('AppointmentWidget', 'JSAnHourSelected', numHour);
    }

    function inArray(needle, haystack) {
        var length = haystack.length;
        for(var i = 0; i < length; i++) {
            if(haystack[i] == needle) return true;
        }
        return false;
    }

    function preperHour(json){
        //var json = '{"Days":[{"ReservedHours":[{"Hour8": 8},{"Hour11": 11},{"Hour16": 16}]}]}';
        document.getElementById('selectedHour').innerHTML = -30;
        obj = JSON.parse(json);
        var forum = obj.Days;
        var rs = parser(forum,0);
        var hourInvalid = parser(rs,1);
        drawHour(hourInvalid)
    }

