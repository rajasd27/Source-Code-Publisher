var commentsHidden = false;
var style;

function toggleVisibility(classname) {
	var elements = document.getElementsByClassName(classname);
    for (var i=0; i<elements.length; i++){
        style = elements[i].style.display;
    }
    
    if (style == "none") {
        for (var i=0; i<elements.length; i++)
            elements[i].style.display = "block";
        
        if (classname == "comments") 
            document.getElementById("button1").innerHTML = "Hide Comments";
        else 
            document.getElementById(classname).innerHTML = "-";
    }
    
    else {
        for (var i=0; i<elements.length; i++)
            elements[i].style.display = "none";
        
        if (classname == "comments") 
            document.getElementById("button1").innerHTML = "Show Comments";
        else 
            document.getElementById(classname).innerHTML = "+";
    }
} 