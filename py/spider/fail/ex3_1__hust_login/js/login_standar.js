$(function() {
	document.cookie = "cas_hash=" + encodeURIComponent(window.location.hash) ;
	if(window.localStorage){
		//重新登录的时候清除掉localStorage
		window.localStorage.clear();
	}
	if(window.sessionStorage){
		//重新登录的时候清除掉sessionStorage
		window.sessionStorage.clear();
	}
	//登录时的账号登录和扫码登录切换
	var lqrcode = new loginQRCode("qrcode",153,153);
	$(".login_box_tab").delegate('a', 'click', function() {
		if(!$(this).hasClass("active")){
			if($(".password").hasClass("active")){
				$("#u_content_1").hide();
				$("#u_content_2").hide();
				$("#qr_content").show();
				lqrcode.generateLoginQRCode(function(result){
					window.location.href = result.redirect_url;
				});
			}else{
				//隐藏账号密码登录的div
				$("#u_content_1").show();
				$("#u_content_2").show();
				$("#qr_content").hide();
				lqrcode.clear();
			}
		}
		$(this).addClass("active").siblings().removeClass("active");
	});
	//登录按钮触发
	$("#index_login_btn").click(function(){
		login();
	});

	//用户名文本域keyup事件
	$("#un").keyup(function(e){
		if(e.which == 13) {
			login();
		}
	}).focus(function(){
		$("#errormsg").hide();
	}).keydown(function(e){
		$("#errormsg").hide();
	});

	//密码文本域keyup事件
	$("#pd").keyup(function(e){
		if(e.which == 13) {
			login();
		}
	}).keydown(function(e){
		$("#errormsg").hide();
	});
	//二维码输入框回车登录
	$("#code").keyup(function(e){
		if(e.which == 13) {
			login();
		}
	})
	//二维码输入框回车登录
	$("#phoneCode").keyup(function(e){
		if(e.which == 13) {
			login();
		}
	})

	//如果有错误信息，则显示
	if($("#errormsghide").text()){
		$("#errormsg").text($("#errormsghide").text()).show();
	}

	//点击记住用户名
	$("#rememberName").change(function(){
		//点击记住账号密码
		if($(this).is(":checked")){
			var $u = $("#un").val() ;
			if($.trim($u)==''){
				$("#errormsg").text(error).show();
				$(this).prop("checked", false);
			}else{
				//不等于空，写cookie
				setCookie('hust_cas_un' , $u , 7);
			}
		}else{
			//反选之后清空cookie
			clearCookie('hust_cas_un');
		}
	});

	//重新获取验证码
	$("#codeImage").click(function(){
		$(this).attr("src", "code?"+Math.random()) ;
	});

	//密码找回的中英文切换
	if($("#change_language").attr("value") == "中文"){
		$("#pwd_url").attr("href",$("#pwd_url").attr("href")+"?locale=en");
	}else{
		$("#pwd_url").attr("href",$("#pwd_url").attr("href")+"?locale=zh_CN");
	}
	$("#change_language").unbind("click").click(function(){
		var re=eval('/(locale=)([^&]*)/gi');
		var url = window.location.href;
		if($("#change_language").attr("value") == "中文"){
			if(url.indexOf("locale") >= 0 ) {
				url=url.replace(re,'locale=zh_CN');
				location.href=url;
			}else{
				if(url.indexOf("?") >= 0){
					location.href=url+"&locale=zh_CN";
				}else{
					location.href=url+"?locale=zh_CN";
				}
			}
		}else if($("#change_language").attr("value") == "English") {
			if(url.indexOf("locale") >= 0 ) {
				url=url.replace(re,'locale=en');
				location.href=url;
			}else{
				if(url.indexOf("?") >= 0){
					location.href=url+"&locale=en";
				}else{
					location.href=url+"?locale=en";
				}
			}
		}
	});
});

function login(){

	var $u = $("#un") , $p=$("#pd");

	var u = $u.val().trim();
	if(u==""){
		$u.focus();
		return ;
	}

	var p = $p.val().trim();
	if(p==""){
		$p.focus();
		return ;
	}

	if(Boolean($("#phoneCode"))){
		let phonecode = $("#phoneCode").val();
		if(phonecode==''){
			$("#phoneCode").focus();
			return;
		}else{
			$("#phoneCode2").val(phonecode);
		}
	}


	$u.attr("disabled","disabled");
	$p.attr("disabled","disabled");
	$u.prop("disabled","disabled");
	$p.prop("disabled","disabled");
	
	if($("#code").val()){
		$("#code2").val($("#code").val());
	}

	//获取key
	$.ajax({
		 url : "rsa",
		 dataType : "json",
		 type : "POST",
		 success:function(data){
			var encrypt = new JSEncrypt();
			encrypt.setPublicKey(data.publicKey);
			$("#ul").val(encrypt.encrypt(u));
			$("#pl").val(encrypt.encrypt(p));
			$("#loginForm")[0].submit();
		 }
	});
	
}


//重新获取验证码
function refreshCodeImg(){
	$("#codeImage").attr("src", "code?"+Math.random()) ;
}

//设置cookie
function setCookie(cname, cvalue, exdays) {
	var d = new Date();
	d.setTime(d.getTime() + (exdays*24*60*60*1000));
	var expires = "expires="+d.toUTCString();
	document.cookie = cname + "=" + cvalue + "; " + expires;
}

//获取cookie
function getCookie(cname) {
	var name = cname + "=";
	var ca = document.cookie.split(';');
	for(var i=0; i<ca.length; i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1);
		if (c.indexOf(name) != -1) return c.substring(name.length, c.length);
	}
	return "";
}

//清除cookie  
function clearCookie(name) {
	setCookie(name, "", -1);
}  