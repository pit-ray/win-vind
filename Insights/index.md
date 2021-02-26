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
        var sp = item.name.split('.');
        var version = sp[0] + '.' + sp[1] + '.x';

        var idx = names.indexOf(version);
        console.log(idx) ;
        if(idx == -1) {
          names.push(version);
        }

        console.log(item.name) ;
        var cnt = 0 ;
        for(var a of item.assets) {
          cnt += a.download_count;
        }

        console.log(cnt) ;
        if(idx == -1) {
          console.log('new') ;
          counts.push(cnt);
          console.log(counts[counts.length - 1]) ;
        }
        else {
          counts[idx] += cnt;
          console.log(counts[idx]) ;
        }
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
};
request.send();
</script>
