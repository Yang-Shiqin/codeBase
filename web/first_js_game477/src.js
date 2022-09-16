
let start_time = new Date;
let end_time = new Date;

let engine = Matter.Engine.create();
let render = Matter.Render.create({
    element:document.body,
    engine:engine,
    options: {
        width: 1200,
        height: 600, 
        wireframes: false
    }
});

let runner = Matter.Runner.create();  // 时间
Matter.Runner.run(runner, engine);

let mouse = Matter.Mouse.create(render.canvas);
let mouseConstraint = Matter.MouseConstraint.create(engine, {
    mouse: mouse,
    constraint: {
        render: {visible: false}
    }
});
render.mouse = mouse;


let layer0 = Matter.Bodies.rectangle(1150, 570, 100, 15, { isStatic: true });
let layer1_1 = Matter.Bodies.rectangle(400, 495, 1000, 15, { isStatic: true });
let layer1_2 = Matter.Bodies.rectangle(1150, 495, 100, 15, { isStatic: true });
let layer2 = Matter.Bodies.rectangle(650, 300, 1100, 15, { isStatic: true });
let layer2_0 = Matter.Bodies.rectangle(50, 480, 100, 15, { isStatic: true, render: {fillStyle: '#060a19'}});

let layer3_0 = Matter.Bodies.rectangle(1100, 100, 200, 15, { isStatic: true });
let layer3_1 = Matter.Bodies.rectangle(50, 100, 100, 15, { isStatic: true });
let layer3_2 = Matter.Bodies.rectangle(500, 260, 100, 15, { isStatic: true });
let layer3_3 = Matter.Bodies.rectangle(400, 230, 100, 15, { isStatic: true });
let layer3_4 = Matter.Bodies.rectangle(300, 200, 70, 15, { isStatic: true });
let layer3_5 = Matter.Bodies.rectangle(200, 159, 100, 15, { isStatic: true });
let layer3_6 = Matter.Bodies.rectangle(120, 135, 70, 15, { isStatic: true });

let love = Matter.Vertices.fromPath('0 24 15 45 36 60 57 45 72 24 66 6 60 0 48 0 36 12 24 0 12 0 6 6');
let layer3_love = Matter.Bodies.fromVertices(555, 90, love, { isStatic: true, render: {fillStyle: '#ff1aa9'}});

let layer4_1 = Matter.Bodies.rectangle(20, 40, 40, 5, { isStatic: true });
let layer4_2 = Matter.Bodies.rectangle(40, 70, 40, 5, { isStatic: true });
let layer4_3 = Matter.Bodies.rectangle(540, 40, 100, 5, { isStatic: true, render: {fillStyle: '#ff0000'}});

let me = Matter.Bodies.circle(50, 500, 10, {render: {fillStyle: '#ff1aa9' }});
let master1 = Matter.Bodies.rectangle(800, 300, 30, 30, { isStatic: false });
let master2 = Matter.Bodies.rectangle(300, 300, 35, 35, { isStatic: false });
let master3 = Matter.Bodies.rectangle(400, 400, 5, 5, { isStatic: false });

let stack = Matter.Composites.stack(910, 300, 8, 10, 0, 14, (x, y)=>Matter.Bodies.rectangle(x, y, 15, 15, { isStatic: false }));
let helper_board = Matter.Bodies.rectangle(15, 550, 40, 5, { isStatic: false });
let board = Matter.Bodies.rectangle(389, 570, 25, 5, { isStatic: false });
let sling = Matter.Constraint.create({ 
    pointA: { x: 389, y: 550 }, 
    bodyB: board, 
    stiffness: 0.7
});
  
let u_v = false;
let r_v = false;
let l_v = false;
let counter=0;

document.onkeydown = keyPress;
//在Document对象中注册keyDown事件处理函数
function keyPress(event){  // 方向键控制元素移动函数
    var event = event || window.event;  // 标准化事件对象
    switch(event.keyCode){  // 获取当前按下键盘键的编码
    case 37 :  // 左
        l_v=true;
        break;
    case 38 :  // 上
        u_v=true;
        break;
    case 39 :  // 右
        r_v=true;
        break;
    default:
        break;
    }
    return false;
}

let win=0;
Matter.Events.on(runner, 'afterTick', function(event) {
    counter += 1;
    
    if (r_v){
        Matter.Body.setVelocity(me, {x:2, y:me.velocity.y});
    }
    if (l_v){
        Matter.Body.setVelocity(me, {x:-2, y:me.velocity.y});
    }
    if (u_v && Math.abs(me.velocity.y)<1){
        Matter.Body.setVelocity(me, {x:me.velocity.x,y:-5});
    }
    if (!(r_v||l_v||u_v)){
        Matter.Body.setVelocity(me, {x:me.velocity.x/1.5, y:me.velocity.y});
    }
    if (Math.abs(me.velocity.y)<0.3){
        r_v = false;
        l_v = false;
    }
    u_v = false;
    
    let py = 380 + 90 * Math.sin(engine.timing.timestamp * 0.001);
    Matter.Body.setVelocity(layer2_0, { x: 0, y: py - layer2_0.position.y });
    Matter.Body.setPosition(layer2_0, { x: 50, y: py });
    let py2 = 555 + 400 * Math.sin(engine.timing.timestamp * 0.0001);
    Matter.Body.setVelocity(layer3_love, { x: py2 - layer3_love.position.y, y: 0});
    Matter.Body.setPosition(layer3_love, { x: py2, y: 90 });
    let py3 = 540 + 470 * Math.sin(engine.timing.timestamp * 0.00015+Math.PI);
    Matter.Body.setVelocity(layer4_3, { x: py3 - layer4_3.position.y, y: 0});
    Matter.Body.setPosition(layer4_3, { x: py3, y: 40 });
    // every 1.5 sec
    if (counter==1 || counter==45){
        Matter.Body.setVelocity(master3, { x: Math.random() * 5-2.5, y: -7 });
        Matter.Body.setAngle(master3, -Math.random() * 0.6);
    }
    if (counter >= 60 * 1.5) {
        Matter.Body.setVelocity(master1, { x: Math.random() * 5-2.5, y: -10 });
        Matter.Body.setAngle(master1, -Math.random() * 0.26);

        Matter.Body.setVelocity(master2, { x: Math.random() * 7-3.5, y: -8 });

        // reset counter
        counter = 0;
    }
    if (0==win && me.position.x>=1050 && me.position.y<=100){
        win=1;
        // 1. 没动画，就出现
        // let mydiv=document.getElementById("text1");
        // mydiv.innerHTML+="宝贝za七夕节快乐oooooo!💕";
        // 2. anime动画
        document.getElementById('text2').style.display='block';  // 改为显示
        let path_one = document.querySelectorAll("#text2 path");
        for (let i = 0; i < path_one.length; i++) {
            anime({
                targets: path_one[i],
                delay: i * 100,
                strokeDashoffset: [anime.setDashoffset, 0],
                easing: "easeInOutSine",
                direction: 'alternate',
                duration: 3000,
                loop: 1,
            });
        }
    }
    if(1==win){
        Matter.Body.setVelocity(me, {x: 1, y:0});
    }else{
        end_time = new Date;
    }

    let mydiv2=document.getElementById("time");
    mydiv2.innerHTML=((end_time-start_time)/1000).toFixed(1).toString()+'s';

});

Matter.World.add(engine.world, [mouseConstraint, 
    layer0, layer1_1, layer1_2, layer2, layer2_0, 
    layer3_0, layer3_1, layer3_2, layer3_3, layer3_4, layer3_5, layer3_6, layer3_love,
    layer4_1, layer4_2, layer4_3,
    stack, helper_board, board, sling, 
    master1, master2, master3,
    me]);
Matter.World.add(engine.world, [
    // walls
    Matter.Bodies.rectangle(600, 0, 1200, 30, { isStatic: true }),
    Matter.Bodies.rectangle(189, 600, 378, 30, { isStatic: true }),
    Matter.Bodies.rectangle(800, 600, 800, 30, { isStatic: true }),
    Matter.Bodies.rectangle(1200, 350, 30, 500, { isStatic: true }),
    Matter.Bodies.rectangle(0, 300, 30, 600, { isStatic: true })
]);
Matter.Engine.run(engine);
Matter.Render.run(render);
