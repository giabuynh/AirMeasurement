function act(){
// Get the modal
var modal = document.getElementById("myModal");

// Get the button that opens the modal
var btn = document.getElementById("myBtn");

// Get the <button> element that closes the modal
var button = document.getElementById("butt");

// When the user clicks the button, open the modal 
btn.modal.style.display="block";

// When the user clicks on <button>, close the modal
button.modal.style.display="none";

if (event.target == modal) {
    window.onclick.modal.style.display = "none";
  }
}