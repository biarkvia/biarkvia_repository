<?php

namespace App\Controller\Api;

use App\Entity\Employee;
use App\Operation\EmployeeOperation;
use App\Resource\EmployeeResourceFactory;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;
use Symfony\Component\Serializer\SerializerInterface;
use Symfony\Component\Validator\ValidatorInterface;
use Symfony\Component\HttpFoundation\JsonResponse;

#[Route('/api/employees', name: 'api_employees_')]
class EmployeeController extends AbstractController
{
    public function __construct(
        private EmployeeOperation $employeeOperation,
        private EmployeeResourceFactory $employeeResourceFactory
    ) {}

    #[Route('', name: 'list', methods: ['GET'])]
    public function list(Request $request): Response
    {
        $criteria = [
            'firstName' => $request->query->get('firstName'),
            'lastName' => $request->query->get('lastName'),
            'email' => $request->query->get('email'),
            'phone' => $request->query->get('phone'),
            'website' => $request->query->get('website'),
            'description' => $request->query->get('description'),
            'roleTitles' => $request->query->all('roleTitles'),
        ];
        $criteria = array_filter($criteria, fn($value) => !empty($value));

        $employees = $this->employeeOperation->search($criteria, 'id', 'asc');

        $resources = array_map(
            fn(Employee $employee) => $this->employeeResourceFactory->fromEntity($employee),
            $employees
        );

        return $this->json([
            'data' => $resources,
            'total' => count($resources),
        ]);
    }

    #[Route('/{id<\d+>}', name: 'show', methods: ['GET'])]
    public function show(int $id): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $resource = $this->employeeResourceFactory->fromEntity($employee);

        return $this->json($resource);
    }

    #[Route('', name: 'create', methods: ['POST'])]
    public function create(
        Request $request,
        SerializerInterface $serializer,
        ValidatorInterface $validator
    ): Response {
        $data = json_decode($request->getContent(), true);

        $employee = $serializer->deserialize(json_encode($data), Employee::class, 'json');

        $violations = $validator->validate($employee);
        if (count($violations) > 0) {
            return $this->json(['errors' => (string) $violations], Response::HTTP_BAD_REQUEST);
        }

        $this->employeeOperation->store($employee);
        $resource = $this->employeeResourceFactory->fromEntity($employee);

        return $this->json($resource, Response::HTTP_CREATED);
    }

    #[Route('/{id}', name: 'update', methods: ['PUT'])]
    public function update(
        int $id,
        Request $request,
        SerializerInterface $serializer,
        ValidatorInterface $validator
    ): Response {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $data = json_decode($request->getContent(), true);
        $serializer->deserialize(json_encode($data), Employee::class, 'json', ['object_to_populate' => $employee]);

        $violations = $validator->validate($employee);
        if (count($violations) > 0) {
            return $this->json(['errors' => (string) $violations], Response::HTTP_BAD_REQUEST);
        }

        $this->employeeOperation->store($employee);
        $resource = $this->employeeResourceFactory->fromEntity($employee);

        return $this->json($resource);
    }

    #[Route('/{id}', name: 'delete', methods: ['DELETE'])]
    public function delete(int $id): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $this->employeeOperation->delete($employee);

        return $this->json(['message' => 'Employee deleted successfully'], Response::HTTP_NO_CONTENT);
    }

    #[Route('/autocomplete', name: 'autocomplete', methods: ['GET'])]
    public function autocomplete(Request $request): JsonResponse
    {
        $query = $request->query->get('query', '');
        $employees = $this->employeeOperation->searchByName($query);

        $results = array_map(fn($employee) => [
            'id' => $employee->getId(),
            'fullName' => $employee->getFullName(),
        ], $employees);

        return $this->json(['data' => $results]);
    }
}
