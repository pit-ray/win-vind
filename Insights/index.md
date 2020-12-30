---
layout: default
title: Insights  
nav_order: 6  
descripption: "Insights of win-vind."  
---  

# Insights  

## Download Count  

<canvas id="dl_count" width=400 height=400></canvas>  

<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js"></script>  

<br>    

<script>  
var request = new XMLHttpRequest();
request.open('GET', 'https://api.github.com/repos/pit-ray/win-vind/releases');  

var names = [];
var counts = [];
request.onreadystatechange = function() {
  if(request.readyState == 4) {
    if (request.status == 200) {
      var data = JSON.parse(request.responseText);
      console.log(data);
      for(var item of data) {
        names.push(item.name);
        var count = 0 ;
        for(var a of item.assets) {
          count += a.download_count;
        }
        counts.push(count);
      }
    }
  }
} ;
request.send();

const ctx = document.getElementById('dl_count');
const chart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: names,
    datasets: [
      label: 'DL',
      data: counts
    ]
  }
});
</script>

