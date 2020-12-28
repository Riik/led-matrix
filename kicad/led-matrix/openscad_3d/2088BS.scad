module feetsie(rotation=0) {
    color("white", 1.0) rotate([0, 0, rotation]) {
        cube([0.5,1,1], center=true);
        translate([-0.5, 0.25, 0]){
        cube([0.5,0.5,1], center=true);
        }
    }
}
union(){
    color("white", 1.0) cube([60.0,60.0,9.2], center=true);
    
    translate([0, 30.05, 0]) {
        color("white", 1.0) cube([20.2,0.1,9.2], center=true);
    }
    translate([0, -30.05, 0]) {
        color("white", 1.0) cube([20.2,0.1,9.2], center=true);
    }
    translate([30.05, 0.0, 0]) {
        color("white", 1.0) cube([0.1,20.2,9.2], center=true);
    }
    translate([-30.05, 0.0, 0]) {
        color("white", 1.0) cube([0.1,20.2,9.2], center=true);
    }
    
    translate([25.15, 30.05, 0]) {
        color("white", 1.0) cube([10.0,0.1,9.2], center=true);
    }
    translate([30.05, 25.15, 0]) {
        color("white", 1.0) cube([0.1,10.0,9.2], center=true);
    }
    translate([-25.15, 30.05, 0]) {
        color("white", 1.0) cube([10.0,0.1,9.2], center=true);
    }
    translate([-30.05, 25.15, 0]) {
        color("white", 1.0) cube([0.1,10.0,9.2], center=true);
    }
    translate([-25.15, -30.05, 0]) {
        color("white", 1.0) cube([10.0,0.1,9.2], center=true);
    }
    translate([-30.05, -25.15, 0]) {
        color("white", 1.0) cube([0.1,10.0,9.2], center=true);
    }
    translate([25.15, -30.05, 0]) {
        color("white", 1.0) cube([10.0,0.1,9.2], center=true);
    }
    translate([30.05, -25.15, 0]) {
        color("white", 1.0) cube([0.1,10.0,9.2], center=true);
    }
    
    translate([-30.1, 15.1, 0]) {
        color("white", 1.0) cube([0.2,10,9.2], center=true);
    }
    translate([-30.1, -15.1, 0]) {
        color("white", 1.0) cube([0.2,10,9.2], center=true);
    }
    translate([15.1, 30.1, 0]) {
        color("white", 1.0) cube([10,0.2,9.2], center=true);
    }
    translate([-15.1, 30.1, 0]) {
        color("white", 1.0) cube([10,0.2,9.2], center=true);
    }

translate([29.85, 29.6, -5.1]) {
        feetsie(0);
}

translate([-29.6, 29.85, -5.1]) {
        feetsie(90);
}

translate([-29.85, -29.6, -5.1]) {
        feetsie(180);
}

translate([29.6, -29.85, -5.1]) {
        feetsie(270);
}
// The pins
// The central point of the pin row on either side is (60.2-45.72)/2 = 7.24 (22.86 from the center line) from its closest side. A pin has a diameter of 0.6 mm. A pin has a length of 7 mm. There are eight pins on either side. The spacing (heart to heart) is 2.54 mm.
for (j=[-1, 1]) {
    for (i = [0:7]) {
        translate([8.89 - i * 2.54, j*22.86, -8.1]) {
        color("Olive",1.0) cylinder(r=0.3,h=7,center=true);
        }
    }
}

// The leds themselves are little circles, diameter of 5.0 mm, heart to heart space of 7.62 mm. The heart of one next to a side lies (60.2 - 53.34)/2 = 3.43 mm from that side. This is equal to 30.1 - 3.43 = 26.67 mm from the center line
for (i = [0:7]) {
    for (j = [0:7]) {
        translate([26.67 - i*7.62 , 26.67 - j*7.62, 4.3]) {
            color("white", 1.0) cylinder(d=5, h=1, center=true);
     }
    }
}
// Add a black mask on top
translate([0,0, 4.2]) {
    color("black", 1.0) cube([60.2, 60.2, 1], center=true);
}
}