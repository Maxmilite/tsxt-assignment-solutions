(function () {
    window.animation_config = window.animation_config || {
        generatorDelay: { min: 1, max: 20 },
        isRunning: true,
        speed: 1,
        x_axis_rate: 0.1,
        weight: 0.5,
        size: 1.5,
        z_axis_rate: 0.05,
        color: { b: 255, a: 0.5 },
        wind: 1
    };
    window.animation_config.isRunning = true;

    let canvas = document.getElementById('snow-flake-app');
    let ctx = canvas.getContext('2d');
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    const generatorDelay = { min: 1, max: 1 };
    let screenBounds = { lower: 0, upper: canvas.width };

    window.onfocus = function () {
        //uncomment for performance
        //window.animation_config.isRunning = true;
    };
    window.onblur = function () {
        //uncomment for performance
        //window.animation_config.isRunning = false;
    };

    window.onresize = function () {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
        screenBounds.upper = canvas.width;
        // init();
        // animate();
    };
    window.onreset = function () {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
        screenBounds.upper = canvas.width;
        // init();
        // animate();
    };

    // Waiting for recoding.
    // Recoded 2021-1-17 17:06


    let particleArray = [];

    function randomInt(min = 0, max = 1) {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }
    function randomFloat(min = 0, max = 1) {
        return (Math.random() * (max - min)) + min;  //Math.random() * (max - min + 1) + min;
    }
    function randomColor(brightness, alpha) {
        return 'rgba(' + randomInt(brightness, 255) + ',' + randomInt(brightness, 255) + ',' + randomInt(brightness, 255) + ',' + alpha + ')';
    }

    class Particle {
        constructor(pos, vector, z_index, size, color, weight) {
            this.pos = pos;
            this.vector = vector;
            this.z_index = z_index % 5;
            this.color = color;
            this.size = size % 50;
            this.weight = weight % 50;
        }

        get displaySize() {
            return Math.sqrt(this.z_index * this.size) * window.animation_config.size;
        }

        draw() {
            if (this.death || !(this.pos.x >= 0 && this.pos.x <= canvas.width)) return;
            ctx.beginPath();
            ctx.arc(this.pos.x, this.pos.y, this.displaySize, 0, Math.PI * 2);
            ctx.fillStyle = this.color;
            ctx.fill();
        }

        update() {
            this.vector.z += randomFloat(-0.1, 0.1);
            this.z_index += this.vector.z * window.animation_config.z_axis_rate;
            if (this.z_index <= 0) this.z_index = Math.abs(this.z_index);
            if (this.pos.y > canvas.height - this.displaySize || !(this.pos.x >= screenBounds.lower && this.pos.x <= screenBounds.upper) || this.displaySize <= 0) {
                this.death = true;
                return;
            }

            this.vector.x += (randomFloat(-0.3, 0.3) * this.z_index) * window.animation_config.x_axis_rate;
            this.vector.y = Math.sqrt(this.weight * this.size * this.z_index) * window.animation_config.speed;


            this.pos.x += this.vector.x + window.animation_config.wind;
            this.pos.y += this.vector.y;
        }
    }

    function init() {
        particleArray = [];

        function generateRandomParticle() {
            if (window.animation_config.isRunning) {
                if (window.animation_config.wind > 0) {
                    screenBounds.lower = (window.animation_config.wind * 200) * -1;
                    screenBounds.upper = canvas.width;
                }
                else if (window.animation_config.wind < 0) {
                    screenBounds.lower = 0;
                    screenBounds.upper = canvas.width + (window.animation_config.wind * -200);
                }
                let pos = { x: randomInt(screenBounds.lower, screenBounds.upper), y: 0 };
                let vector = { x: randomFloat(-0.3, 0.3), y: randomFloat(-5, 5), z: randomFloat(-0.1, 0.1) };
                let s = randomFloat(0.1, 5);
                let z = randomFloat(0.2, 5);
                let c = randomColor(window.animation_config.color.b, window.animation_config.color.a);
                let w = window.animation_config.weight;
                particleArray.push(new Particle(pos, vector, z, s, c, w));
            }
            setTimeout(generateRandomParticle, randomInt(window.animation_config.generatorDelay.min, window.animation_config.generatorDelay.max));
        }


        generateRandomParticle();
    }

    function animate() {

        if (window.animation_config.isRunning) {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            particleArray = particleArray.filter(particle => !particle.death);
            for (let i = 0; i < particleArray.length; i++) {
                particleArray[i].update();
                particleArray[i].draw();
            }
        }
        requestAnimationFrame(animate);
    }

    init();
    animate();
})();