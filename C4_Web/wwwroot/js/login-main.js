const inputs = document.querySelectorAll('.input-field');
const toggle_btn=document.querySelectorAll('.toggle');
const main=document.querySelector('main');
const bullets=document.querySelectorAll('.bullets span');
const images=document.querySelectorAll('.image');

inputs.forEach(inp=>{
    inp.addEventListener('focus',()=>{
        inp .classList.add('active');
    })
    inp.addEventListener('blur',()=>{
        if(inp.value!='')return;
        inp.classList.remove('active');
    })
})

toggle_btn.forEach(btn=>{
    btn.addEventListener('click',()=>{
        main.classList.toggle('sign-up-mode');
    })
})

function moveSlider(){
    let index=this.dataset.value;
    let currentImg=document.querySelector('.img-${index}');
    images.forEach(img=>{
        img.classList.remove('show');
    })
    currentImg.classList.add('show');
    const textSlider=document.querySelector('.text-group');
    textSlider.style.transform='translateY(${-(index-1)*2.2}rem)'
    bullets.forEach(bullet=>{
        bullet.classList.remove('active');
    })
    this.classList.add('active')
}

bullets.forEach(bullet=>{
    bullet.addEventListener('click',moveSlider);
})

@media(max-width:850px){
    .box{
        height: auto;
        max-width: 550px;
        overflow:hidden;
    }
    .inner-box{
        position:StaticRange;
        transform:none;
        width:revert;
        padding:2rem;
    }

    .forms-wrap{
        position:revert;
        width:100%;
        height:100%;
    }

    form{
        max-width:revert;
        padding:1.5rem 2.5rem 2rem;
        transition:transform 0.8s ease-in-out,opacity 0.45s linear;
    }
    .heading{
    }
}