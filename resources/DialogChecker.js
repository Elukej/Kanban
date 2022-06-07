
function resetDialog(columnLayout) {
    for (let i = 0; i < columnLayout.children.length; i++) {
       columnLayout.children[i].text = "";
       columnLayout.children[i].warningVisible = false
    }
}   

function checkDialog(columnLayout) { //return object with fields holding true or error string for fields
    let messages = { 
                    "title" : "Field cannot be empty",
                    "description" : "Field cannot be empty",
                    "startDate" : "Field cannot be empty",
                    "estimation" : "Field cannot be empty",
                    "status" : "Field cannot be empty"
                  };
    let status = ["To Do", "In Progress", "Blocked", "Done"]
    let dates = [];
    let i = 0;
    for (var mes in messages) { 
        if (columnLayout.children[i].text !== "") {
            messages[mes] = true;
            if (mes === "startDate" || mes === "estimation") {
                messages[mes] = checkDate(columnLayout.children[i].text);
                if (messages[mes] === true)  {
                    let date = columnLayout.children[i].text.split("-")
                    dates.push(new Date(date[0], date[1], date[2]))
                    //dates.push(new Date(columnLayout.children[i].text)) //for some very weird reason, this was working for already made
                    //tasks but wasnt working when adding a new task. the date would be checked correctly, but the constructor would create an
                    //Invalide date. could be even a qt bug but  i don't know. Strange behavior anyway
                }        
            }
        }
        i++;
    }

    if (!(status.includes(columnLayout.children[4].text))) {
        messages["status"] = "Valid input is: To Do, In Progress, Blocked, Done"
    }

    if (messages["startDate"] === true && 
        messages["estimation"] === true &&
        dates[0] > dates[1]) 
    {
        messages["estimation"] = "Estimation needs to be after StartDate"
    }

    return messages;
}

function checkDate(someDate) {
    let dateSplit = someDate.split('-');
    let error = "Wrong! Date Needs to be in format yyyy-MM-dd"
    let errorYear = "Year needs to be between 2000 and 2099"
    let errorMonth = "Month needs to be between 1 and 12 inclusive"
    let errorDay = "Day out of scope for this month"
    if (dateSplit.length !== 3) return error;
    
    let year = Number(dateSplit[0]);
    if (year === NaN || !(year % 1 === 0)) return error
    if (year < 2000 || year > 2099) return errorYear
    
    let month = Number(dateSplit[1])
    if (month === NaN || !(month % 1 === 0)) return error
    if (month < 1 || month > 12) return errorMonth

    let day = Number(dateSplit[2])
    if (day === NaN || !(day % 1 === 0)) return error;
    if (day < 1) return errorDay
    switch(month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: if(day > 31) return errorDay
                 else break;
        case 4:
        case 6:
        case 9:
        case 11: if (day > 30) return errorDay
                 else break;
        case 2:  if (year % 4 === 0 && day > 29) return errorDay
                 else if (year % 4 !== 0 && day > 28) return errorDay
                 else break;
        default: return error
    } 

    return true    
}