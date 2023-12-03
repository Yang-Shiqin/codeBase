const $ = require('jquery.min.js');
//const $ = require('~/.nvm/versions/node/v18.17.1/lib/node_modules/jquery');
function processLogin(u, p) {
  return new Promise(function(resolve, reject) {
    $.ajax({
      url: "rsa",
      dataType: "json",
      type: "POST",
      success: function(data) {
        var encrypt = new JSEncrypt();
        encrypt.setPublicKey(data.publicKey);
        var ul = encrypt.encrypt(u);
        var pl = encrypt.encrypt(p);
        resolve({ ul: ul, pl: pl, loginForm: "#loginForm" });
      },
      error: function() {
        reject("请求失败");
      }
    });
  });
}

// 使用示例
processLogin("username", "password")
  .then(function(result) {
    var ul = result.ul;
    var pl = result.pl;
    var loginForm = result.loginForm;

    // 进行后续操作
    // ...
  })
  .catch(function(error) {
    // 处理请求失败
    console.log(error);
  });
