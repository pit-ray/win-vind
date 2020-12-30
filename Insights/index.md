---
layout: default
title: Insights  
nav_order: 6  
descripption: "Insights of win-vind."  
---  

# Insights  

## Download Count  

<canvas id="myChart" width=400 height=400></canvas>  

<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js"></script>  

<br>    

<script>  
var request = new XMLHttpRequest();
request.open('GET', 'https://api.github.com/repos/pit-ray/win-vind/releases');  

request.onreadystatechange = function() {

  if(request.readyState != 4) {
  
  } else if (request.status == 200) {
    var data = JSON.parse(request.responseText);
    console.log(data);  
  }
} ;
</script>

