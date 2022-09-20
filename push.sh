_t=$(TZ=UTC-8 date '+%Y-%m-%d-%H-%M-%S')
_up="(from: Eplankton)"
git add .
git commit -m "$_t $_up"
git push