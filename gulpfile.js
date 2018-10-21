const gulp = require('gulp')
const uglify = require('gulp-uglify')
const rename = require('gulp-rename')
const pump = require('pump')
const clean = require('gulp-clean')
const CacheBuster = require('gulp-cachebust')

const cachebust = new CacheBuster()

gulp.task('build-js', cb => {
  pump(
    [
      gulp.src('dist/index.js'),
      uglify(),
      cachebust.resources(),
      gulp.dest('dist'),
    ],
    cb
  )
})

gulp.task('build-html', ['build-js'], cb => {
  pump(
    [
      gulp.src('public/index.prod.html'),
      cachebust.references(),
      rename('index.html'),
      gulp.dest('dist'),
    ],
    cb
  )
})

gulp.task('cleanup', ['build-html', 'build-js'], cb => {
  pump([gulp.src('dist/index.js'), clean()])
})

gulp.task('build', ['build-html', 'cleanup'])
