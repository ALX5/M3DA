    // -----------------------------------------------------------------------
/// \brief Calcule un terme de contrainte a partir d'une homographie.
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \param i: premiere colonne.
/// \param j: deuxieme colonne.
/// \return vecteur definissant le terme de contrainte.
// -----------------------------------------------------------------------
function v = ZhangConstraintTerm(H, i, j)
  v1 = H(i,1)*H(j,1);
  v2 = H(i,1)*H(j,2)+H(i,2)*H(j,1);
  v3 = H(i,2)*H(j,2);
  v4 = H(i,3)*H(j,1)+H(i,1)*H(j,3);
  v5 = H(i,3)*H(j,2)+H(i,2)*H(j,3);
  v6 = H(i,3)*H(j,3);
  v = [v1,v2,v3,v4,v5,v6];
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule deux equations de contrainte a partir d'une homographie
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \return matrice 2*6 definissant les deux contraintes.
// -----------------------------------------------------------------------
function v = ZhangConstraints(H)
  v = [ZhangConstraintTerm(H, 1, 2); ...
    ZhangConstraintTerm(H, 1, 1) - ZhangConstraintTerm(H, 2, 2)];
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres intrinseques.
///
/// \param b: vecteur resultant de l'optimisation de Zhang.
/// \return matrice 3*3 des parametres intrinseques.
// -----------------------------------------------------------------------
function A = IntrinsicMatrix(b)  
  // b=[B(1,1);B(1,2);B(2,2);B(1,3);B(2,3);B(3,3)]
  B11=b(1,:);
  B12=b(2,:);
  B22=b(3,:);
  B13=b(4,:);
  B23=b(5,:);
  B33=b(6,:);
  v0 = (B12*B13-B11*B23)/(B11*B22-B12*B12);
  lambda = B33-((B13^2+v0*(B12*B13-B11*B23))/B11);
  alpha = sqrt(lambda/B11);
  beta = sqrt(lambda*B11/(B11*B22-B12^2));
  gamma = -B12*alpha^2*beta/lambda;
  u0 = gamma*v0/beta-B13*alpha^2/lambda;
  A=[alpha,gamma,u0;0,beta,v0;0,0,1];
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres extrinseques.
///
/// \param iA: inverse de la matrice intrinseque.
/// \param H: matrice 3*3 definissant l'homographie.
/// \return matrice 3*4 des parametres extrinseques.
// -----------------------------------------------------------------------
function E = ExtrinsicMatrix(iA, H)
  h1 = H(:,1);
  h2 = H(:,2);
  h3 = H(:,3);
  lambda = 1/norm(iA*h1);
  r1 = lambda*iA*h1;
  r2 = lambda*iA*h2;
  r3 = cross_prod(r1,r2);
  t = lambda*iA*h3;
  R = [r1,r2,r3];
  E = [R,t];
endfunction

function w=cross_prod(u,v)
    w=zeros(3,1)
    D=[u(:),v(:)]
    w(1)=det([[1;0;0],D])
    w(2)=det([[0;1;0],D])
    w(3)=det([[0;0;1],D])
endfunction

