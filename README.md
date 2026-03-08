# C++ Hello World no Kubernetes

Projeto de servidor TCP Hello World em C++ rodando em Kubernetes local com ArgoCD.

## Estrutura

```
k8s/
├── app/                      # Código da aplicação
│   ├── main.cpp              # Servidor TCP C++
│   ├── Dockerfile            # Build multi-stage com GCC
│   └── .dockerignore
├── k8s-manifests/            # Manifests Kubernetes
│   ├── deployment.yaml       # Deployment da aplicação
│   ├── service.yaml          # Service NodePort
│   └── kustomization.yaml    # Kustomize config
├── kind-config.yaml          # Configuração do cluster Kind
└── argocd-app.yaml           # Application ArgoCD (template)
```

## Acessos

### Servidor C++ TCP
- **URL**: tcp://localhost:30080
- **Resposta**: "Hello World from C++!"
- **Healthcheck**: TCP Socket Probe na porta 8000

### ArgoCD UI
- **URL**: https://localhost:8080
- **Usuário**: `admin`
- **Senha**: `0XajmEgsIo-dZZ3W`

## Comandos Úteis

### Verificar pods
```powershell
kubectl get pods
```

### Ver logs da aplicação
```powershell
kubectl logs -l app=cpp-hello -f
```

### Reiniciar deployment
```powershell
kubectl rollout restart deployment/cpp-hello
```

### Acessar ArgoCD (se o port-forward parou)
```powershell
kubectl port-forward svc/argocd-server -n argocd 8080:443
```

### Rebuild da imagem
```powershell
cd app
docker build -t cpp-hello:v1 .
kind load docker-image cpp-hello:v1 --name cpp-cluster
kubectl rollout restart deployment/cpp-hello
```

### Deletar cluster
```powershell
kind delete cluster --name cpp-cluster
```
