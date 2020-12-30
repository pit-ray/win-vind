---
layout: default
title: Insights  
nav_order: 6  
descripption: "Insights of win-vind."  
---  

# Insights  

## Download Count  

<canvas id="dl_count"></canvas>  

<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js"></script>  

<br>    

<script>  
var request = new XMLHttpRequest();
request.open('GET', 'https://api.github.com/repos/pit-ray/win-vind/releases');  

request.onreadystatechange = function() {
  var names = [];
  var counts = [];
  
  if(request.readyState == 4) {
    if (request.status == 200) {
      var data = JSON.parse(request.responseText);
      for(var item of data) {
        names.push(item.name);
        var cnt = 0 ;
        for(var a of item.assets) {
          cnt += a.download_count;
        }
        counts.push(cnt);
      }
    }
  }
  
  const ctx = document.getElementById('dl_count');
  const chart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: names.reverse(),
      datasets: [
        {
          label: 'count',
          data: counts.reverse()
        }
      ]
    },
    options: {
         legend: {
            display: false
         }
    }
  });
} ;
request.send();
</script>

