function InitHome() {

}

function onMenu(id) {
    var num = id.substr(4, id.length)
    document.getElementById('menuLink' + num).style.display = 'block';
   
}

function outMenu(id) {
    var num = id.substr(4, id.length)
    document.getElementById('menuLink' + num).style.display = 'none';
}

function OnMenuClicked(id) {
    switch(id) {
        case 1:
            Wt.emit('HomePageWidget', 'JSRegister');
            break;
        case 2:
            window.location.replace('./?lang=fa&url=C0099445C296B98AEE21E3EEEFCD6748276418F2');
            break;
        case 3:
            window.location.replace('./?lang=fa&url=3CA4AE7F2C92A1AD2B66396E3323E4D63D4E1ECA');
            break;
        case 4:
            Wt.emit('HomePageWidget', 'JSSubmitIdea');
            break;
        case 5:
            Wt.emit('HomePageWidget', 'JSSubmitApp');
            break;
        default:
            break;
    }
}


