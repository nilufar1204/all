

const textContainer = document.getElementById('text-container');
  const readMoreBtn = document.getElementById('read-more-btn');

  readMoreBtn.addEventListener('click', () => {
    textContainer.classList.toggle('expanded');
    if (textContainer.classList.contains('expanded')) {
      readMoreBtn.textContent = 'Read Less👆';
    } else {
      readMoreBtn.textContent = 'Read More👇';
    }
  });


function toggleAnswer(id) {
  var answer = document.getElementById(id);
  if (answer.style.display === "none") {
    answer.style.display = "block";
  } else {
    answer.style.display = "none";
  }
}